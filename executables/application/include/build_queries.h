#pragma once

#include <game_tui/Queries.h>
#include <game_tui/session/Application.h>

namespace cpp_warships::application {
    /** @brief The read-only questions a screen may ask about the session it is drawing. */
    struct SessionQueries {
        game_tui::MatchQuery match;
        game_tui::MatchInProgressQuery hasMatch;
        game_tui::SavedMatchQuery hasSavedMatch;
        game_tui::ThemeQuery theme;
    };

    /** @brief Questions that read @p session, which must outlive the answers. */
    [[nodiscard]] SessionQueries buildQueries(const game_tui::Application& session);
} // namespace cpp_warships::application
