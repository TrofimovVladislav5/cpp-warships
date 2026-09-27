#pragma once

#include <functional>

#include <game_core/Board.h>
#include <game_flow/Match.h>
#include <game_tui/Theme.h>

namespace cpp_warships::game_tui {
    /** @brief Read-only reach into the match in play, so a screen can draw it but never change it.
     *  Changing the match is an intent's job, and this const reference is what enforces that. */
    using MatchQuery = std::function<const game_flow::Match&()>;

    /** @brief Read-only reach into whichever board a screen is working against. */
    using BoardQuery = std::function<const game_core::Board&()>;

    /** @brief Whether there is a match to go back to, asked without reaching for it. */
    using MatchInProgressQuery = std::function<bool()>;

    /** @brief The colours everything is dressed in, asked afresh so a change is picked up. */
    using ThemeQuery = std::function<const Theme&()>;
} // namespace cpp_warships::game_tui
