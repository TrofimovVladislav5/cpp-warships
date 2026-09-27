#include <run_game.h>

#include <memory>

#include <build_intent_sink.h>
#include <build_navigator.h>
#include <build_queries.h>
#include <build_session.h>
#include <build_shell.h>
#include <build_views.h>

#include <game_tui/intents/IntentContext.h>
#include <game_tui/screens/ScreenNavigator.h>

namespace cpp_warships::application {
    void runGame(game_flow::RandomEngine& randomEngine, const ShellKind shellKind) {
        game_tui::Application session = buildSession(randomEngine);
        const SessionQueries queries = buildQueries(session);

        const game_tui::ViewFactory views = buildViewFactory(shellKind);
        const std::unique_ptr<game_tui::Shell> shell = buildShell(shellKind, queries.theme);
        game_tui::ScreenNavigator navigator;
        const game_tui::IntentContext context{
                .session = session,
                .navigator = navigator,
                .shell = *shell
        };

        const game_tui::IntentSink intentSink = buildIntentSink(context);
        navigator = buildNavigator(intentSink, session, queries, views);
        shell->run(navigator);
    }
} // namespace cpp_warships::application
