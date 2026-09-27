#pragma once

#include <cstddef>
#include <deque>

#include <game_flow/MatchEvent.h>

namespace cpp_warships::game_tui {
    /** @brief The running story of a match, oldest first, trimmed to what a panel can show.
     *  The match forgets its events when drained, so what is kept is kept here. */
    class BattleJournal {
    public:
        void absorb(const game_flow::MatchEventLog& events);
        void clear() noexcept;

        [[nodiscard]] const std::deque<game_flow::MatchEvent>& entries() const noexcept;
        [[nodiscard]] bool isEmpty() const noexcept;

    private:
        std::deque<game_flow::MatchEvent> entries_;
    };
} // namespace cpp_warships::game_tui
