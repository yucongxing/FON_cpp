#pragma once

#include <chrono>
#include <vector>

struct FocusEvent {
    std::chrono::steady_clock::time_point timestamp;
    enum class Type { LOST, REGAINED } type;
};

class FocusScorer {
public:
    void update(bool is_focused);
    bool isFocused() const;
    int  realtimeScore() const;  // 0-100, 60s rolling window
    void reset();

private:
    static constexpr auto BLINK_THRESHOLD = std::chrono::milliseconds(300);
    static constexpr auto SCORE_WINDOW    = std::chrono::seconds(60);

    bool m_focused         = true;
    bool m_pending_unfocus = false;
    std::chrono::steady_clock::time_point m_pending_since;
    std::chrono::steady_clock::time_point m_session_start;

    std::vector<FocusEvent> m_events;
};
