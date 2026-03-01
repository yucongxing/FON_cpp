#include "FocusScorer.h"

#include <algorithm>

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
    bool signal_focused = !analysis.faces.empty() && analysis.faces[0].eyes_open;
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

    // Prune events older than SCORE_WINDOW, keeping one before the boundary
    // so realtimeScore() can reconstruct the state at window_start.
    auto cutoff = now - SCORE_WINDOW;
    auto first_in_window = std::lower_bound(
        m_events.begin(), m_events.end(), cutoff,
        [](const FocusEvent &e, const time_point &t) { return e.timestamp < t; });
    if (first_in_window != m_events.begin()) {
        m_events.erase(m_events.begin(), std::prev(first_in_window));
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

