#pragma once

#include <application/core/Coordinate.h>

namespace cpp_warships::head {
    /** @brief Where the player is aiming, and how far back through the log they have scrolled. */
    struct BattleState {
        core::Coordinate target;

        /** @brief How many of the newest log lines are scrolled past; zero follows the action. */
        int logScroll = 0;
    };

    /** @brief How many lines of the log are on show at once, however long the story gets. */
    inline constexpr int LOG_VISIBLE_LINES = 10;

    /** @brief The furthest back a log of @p entryCount lines scrolls in its window. */
    [[nodiscard]] int furthestLogScroll(int entryCount);
} // namespace cpp_warships::head
