#pragma once

#include <build_queries.h>

#include <game_tui/intents/Intent.h>
#include <game_tui/screens/ScreenNavigator.h>
#include <game_tui/session/Application.h>
#include <game_tui/views/ViewFactory.h>

namespace cpp_warships::application {
    /** @brief A navigator holding every screen the game can show.
     *  This is the one place that names which screens exist and what each is given.
     *  @p session and @p queries must outlive the navigator, as the screens read them. */
    [[nodiscard]] game_tui::ScreenNavigator buildNavigator(
            const game_tui::IntentSink& intentSink,
            const game_tui::Application& session,
            const SessionQueries& queries,
            const game_tui::ViewFactory& views
    );
} // namespace cpp_warships::application
