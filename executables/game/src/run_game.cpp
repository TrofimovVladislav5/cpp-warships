#include <run_game.h>

#include <memory>

#include <build_game.h>
#include <build_intent_sink.h>
#include <build_navigator.h>
#include <build_queries.h>
#include <build_save_archive.h>
#include <build_shell.h>
#include <build_views.h>

#include <application/head/ThemeSelection.h>
#include <application/head/intents/IntentContext.h>
#include <application/head/screens/ScreenNavigator.h>
#include <application/model/ApplicationContext.h>

namespace cpp_warships::application {
    void runGame(flow::RandomEngine& randomEngine, const ShellKind shellKind) {
        const SaveLibrary saves = buildSaveLibrary(defaultSaveDirectory());
        const std::unique_ptr<model::WarshipsGame> game =
                buildGame(randomEngine, *saves.archive);
        model::ApplicationContext application{*game};

        head::ThemeSelection theme;
        const SessionQueries queries = buildQueries(*game, theme);

        const head::ViewFactory views = buildViewFactory(shellKind);
        const std::unique_ptr<head::Shell> shell = buildShell(shellKind, queries.theme);
        head::ScreenNavigator navigator;
        const head::IntentContext context{
                .application = application,
                .navigator = navigator,
                .shell = *shell,
                .theme = theme
        };

        const head::IntentSink intentSink = buildIntentSink(context);
        navigator = buildNavigator(intentSink, *game, queries, views);
        shell->run(navigator);
    }
} // namespace cpp_warships::application
