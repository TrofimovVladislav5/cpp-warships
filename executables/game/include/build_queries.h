#pragma once

#include <application/head/Queries.h>
#include <application/head/ThemeSelection.h>
#include <application/model/WarshipsGame.h>

namespace cpp_warships::application {
    /** @brief The read-only questions a screen may ask about the game it is drawing. */
    struct SessionQueries {
        head::MatchQuery match;
        head::MatchInProgressQuery hasMatch;
        head::SavedMatchQuery hasSavedMatch;
        head::ThemeQuery theme;
    };

    /** @brief Questions that read @p game and @p theme, which must outlive the answers. */
    [[nodiscard]] SessionQueries buildQueries(
            const model::WarshipsGame& game,
            const head::ThemeSelection& theme
    );
} // namespace cpp_warships::application
