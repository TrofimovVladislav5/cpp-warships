#include <run_game.h>

#include <memory>

#include <build_intent_sink.h>
#include <build_navigator.h>
#include <build_queries.h>
#include <build_save_archive.h>
#include <build_session.h>
#include <build_shell.h>
#include <build_views.h>

#include <application/head/intents/IntentContext.h>
#include <application/head/screens/ScreenNavigator.h>

namespace cpp_warships::application {
    void runGame(flow::RandomEngine& randomEngine, const ShellKind shellKind) {
        const SaveLibrary saves = buildSaveLibrary(defaultSaveDirectory());
        head::Application session = buildSession(randomEngine, *saves.archive);
        const SessionQueries queries = buildQueries(session);

        const head::ViewFactory views = buildViewFactory(shellKind);
        const std::unique_ptr<head::Shell> shell = buildShell(shellKind, queries.theme);
        head::ScreenNavigator navigator;
        const head::IntentContext context{
                .session = session,
                .navigator = navigator,
                .shell = *shell
        };

        const head::IntentSink intentSink = buildIntentSink(context);
        navigator = buildNavigator(intentSink, session, queries, views);
        shell->run(navigator);
    }
} // namespace cpp_warships::application
