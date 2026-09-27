#include <build_navigator.h>

#include <memory>

#include <application/head/screens/BattleScreen.h>
#include <application/head/screens/MenuScreen.h>
#include <application/head/screens/PlacementScreen.h>

namespace cpp_warships::application {
    head::ScreenNavigator buildNavigator(
            const head::IntentSink& intentSink,
            const head::Application& session,
            const SessionQueries& queries,
            const head::ViewFactory& views
    ) {
        head::ScreenNavigator navigator;

        navigator.add(
                std::make_unique<head::MenuScreen>(
                        intentSink,
                        session.theme(),
                        queries.hasMatch,
                        queries.hasSavedMatch,
                        views.menu
                )
        );
        navigator.add(
                std::make_unique<head::PlacementScreen>(
                        intentSink,
                        session.theme(),
                        queries.match,
                        views.placement
                )
        );
        navigator.add(
                std::make_unique<head::BattleScreen>(
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
