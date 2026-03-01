#include "FocusScorer.h"

using Clock      = std::chrono::steady_clock;
using time_point = Clock::time_point;
using duration   = std::chrono::duration<double>;

void FocusScorer::reset() {
    m_events.clear();
    m_focused         = true;
    m_pending_unfocus = false;
    m_session_start   = Clock::now();
}

void FocusScorer::update(const FrameAnalysis &analysis) {
    bool signal_focused = analysis.has_face && analysis.faces[0].eyes_open;
    auto now            = Clock::now();

    if (signal_focused) {
        m_pending_unfocus = false;
        if (!m_focused) {
            m_focused = true;
            m_events.push_back({now, FocusEvent::Type::REGAINED});
        }
    } else {
        if (m_focused) {
            if (!m_pending_unfocus) {
                m_pending_unfocus = true;
                m_pending_since   = now;
            } else if (now - m_pending_since >= BLINK_THRESHOLD) {
                m_focused         = false;
                m_pending_unfocus = false;
                m_events.push_back({m_pending_since, FocusEvent::Type::LOST});
            }
        }
    }
}

bool FocusScorer::isFocused() const {
    return m_focused;
}

int FocusScorer::realtimeScore() const {
    auto now          = Clock::now();
    auto window_start = std::max(now - SCORE_WINDOW, m_session_start);

    // Find state at window_start by scanning events before it
    bool cur_state = true;  // session always starts focused
    for (const auto &e : m_events) {
        if (e.timestamp >= window_start) break;
        cur_state = (e.type == FocusEvent::Type::REGAINED);
    }

    // Sum focused time within window
    duration focused_time{0};
    auto prev = window_start;
    for (const auto &e : m_events) {
        if (e.timestamp < window_start) continue;
        if (cur_state) focused_time += e.timestamp - prev;
        cur_state = (e.type == FocusEvent::Type::REGAINED);
        prev      = e.timestamp;
    }
    if (cur_state) focused_time += now - prev;

    duration window_duration = now - window_start;
    if (window_duration.count() <= 0) return 100;

    return static_cast<int>(focused_time / window_duration * 100);
}

const std::vector<FocusEvent>& FocusScorer::events() const {
    return m_events;
}
