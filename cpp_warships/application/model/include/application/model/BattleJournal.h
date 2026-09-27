#pragma once

#include <cstddef>
#include <deque>

#include <application/flow/MatchEvent.h>

namespace cpp_warships::model {
    /** @brief The running story of a match, oldest first, trimmed to what a panel can show.
     *  The match forgets its events when drained, so what is kept is kept here. */
    class BattleJournal {
    public:
        void absorb(const flow::MatchEventLog& events);
        void clear() noexcept;

        [[nodiscard]] const std::deque<flow::MatchEvent>& entries() const noexcept;
        [[nodiscard]] bool isEmpty() const noexcept;

    private:
        std::deque<flow::MatchEvent> entries_;
    };
} // namespace cpp_warships::model
