#pragma once

#include <application/head/common/Queries.h>

namespace cpp_warships::head::common {
    class ThemeSelection;
}

namespace cpp_warships::model {
    class WarshipsGame;
}

namespace cpp_warships::application {
    /** @brief The read-only questions a screen may ask about the game it is
     * drawing. */
    struct SessionQueries {
        head::common::MatchQuery match;
        head::common::MatchInProgressQuery hasMatch;
        head::common::SavedMatchQuery hasSavedMatch;
        head::common::ThemeQuery theme;
    };

    /** @brief Questions that read @p game and @p theme, which must outlive the
     * answers. */
    [[nodiscard]] SessionQueries buildQueries(
        const model::WarshipsGame& game,
        const head::common::ThemeSelection& theme
    );
}  // namespace cpp_warships::application
