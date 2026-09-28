#pragma once

#include <application/flow/Match.h>
#include <application/head/common/Theme.h>

#include <functional>
#include <string>

namespace cpp_warships::head::common {
    /** @brief Read-only reach into the match in play, so a screen can draw it but never change
     * it. */
    using MatchQuery = std::function<const flow::Match&()>;

    /** @brief Whether there is a match to go back to, asked without reaching
     * for it. */
    using MatchInProgressQuery = std::function<bool()>;

    /** @brief Whether a saved match is there to pick back up, asked without
     * reading it. */
    using SavedMatchQuery = std::function<bool()>;

    /** @brief What the match in play is already called, when it came from a save. */
    using SaveNameQuery = std::function<std::string()>;

    /** @brief The colours everything is dressed in, asked afresh so a change is
     * picked up. */
    using ThemeQuery = std::function<const Theme&()>;
}  // namespace cpp_warships::head::common
