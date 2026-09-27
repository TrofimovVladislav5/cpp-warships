#pragma once

#include <functional>

#include <application/core/Board.h>
#include <application/flow/Match.h>
#include <application/head/Theme.h>

namespace cpp_warships::head {
    /** @brief Read-only reach into the match in play, so a screen can draw it but never change it.
     *  Changing the match is an intent's job, and this const reference is what enforces that. */
    using MatchQuery = std::function<const flow::Match&()>;

    /** @brief Read-only reach into whichever board a screen is working against. */
    using BoardQuery = std::function<const core::Board&()>;

    /** @brief Whether there is a match to go back to, asked without reaching for it. */
    using MatchInProgressQuery = std::function<bool()>;

    /** @brief Whether a saved match is there to pick back up, asked without reading it. */
    using SavedMatchQuery = std::function<bool()>;

    /** @brief The colours everything is dressed in, asked afresh so a change is picked up. */
    using ThemeQuery = std::function<const Theme&()>;
} // namespace cpp_warships::head
