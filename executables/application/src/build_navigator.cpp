#include <build_navigator.h>

#include <memory>

#include <game_tui/screens/BattleScreen.h>
#include <game_tui/screens/MenuScreen.h>
#include <game_tui/screens/PlacementScreen.h>

namespace cpp_warships::application {
    game_tui::ScreenNavigator buildNavigator(
            const game_tui::IntentSink& intentSink,
            const game_tui::Application& session,
            const SessionQueries& queries,
            const game_tui::ViewFactory& views
    ) {
        game_tui::ScreenNavigator navigator;

        navigator.add(
                std::make_unique<game_tui::MenuScreen>(
                        intentSink,
                        session.theme(),
                        queries.hasMatch,
                        queries.hasSavedMatch,
                        views.menu
                )
        );
        navigator.add(
                std::make_unique<game_tui::PlacementScreen>(
                        intentSink,
                        session.theme(),
                        queries.match,
                        views.placement
                )
        );
        navigator.add(
                std::make_unique<game_tui::BattleScreen>(
                        intentSink,
                        session.theme(),
                        queries.match,
                        session.journal(),
                        views.battle
                )
        );

        return navigator;
    }
} // namespace cpp_warships::application
