#include <build_navigator.h>

#include <memory>

#include <application/head/screens/BattleScreen.h>
#include <application/head/screens/MenuScreen.h>
#include <application/head/screens/PlacementScreen.h>

namespace cpp_warships::application {
    head::ScreenNavigator buildNavigator(
            const head::IntentSink& intentSink,
            const model::WarshipsGame& game,
            const SessionQueries& queries,
            const head::ViewFactory& views
    ) {
        head::ScreenNavigator navigator;

        navigator.add(
                std::make_unique<head::MenuScreen>(
                        intentSink,
                        queries.theme(),
                        queries.hasMatch,
                        queries.hasSavedMatch,
                        views.menu
                )
        );
        navigator.add(
                std::make_unique<head::PlacementScreen>(
                        intentSink,
                        queries.theme(),
                        queries.match,
                        views.placement
                )
        );
        navigator.add(
                std::make_unique<head::BattleScreen>(
                        intentSink,
                        queries.theme(),
                        queries.match,
                        game.journal(),
                        views.battle
                )
        );

        return navigator;
    }
} // namespace cpp_warships::application
