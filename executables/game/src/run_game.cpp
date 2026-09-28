#include <application/head/common/PresentationContext.h>
#include <application/head/common/input/EventPipeline.h>
#include <application/model/ApplicationContext.h>
#include <application/model/intents/IntentFactory.h>
#include <application/model/intents/IntentProcessor.h>
#include <application/model/scenarios/SyncScenarioQueue.h>
#include <build_event_pipeline.h>
#include <build_game.h>
#include <build_queries.h>
#include <build_renderers.h>
#include <build_save_archive.h>
#include <build_shell.h>
#include <run_game.h>

#include <memory>

namespace cpp_warships::application {
    void runGame(flow::RandomEngine& randomEngine, const ShellKind shellKind) {
        const SaveLibrary saves = buildSaveLibrary(defaultSaveDirectory());
        const std::unique_ptr<model::WarshipsGame> game = buildGame(randomEngine, *saves.archive);

        model::ApplicationContext application{*game};
        const model::IntentFactory intents{application};
        model::IntentProcessor processor{application};
        model::SyncScenarioQueue scenarios{processor};

        head::PresentationContext context{application};
        const SessionQueries queries = buildQueries(*game, context.themeSelection());

        head::RendererSet renderers = buildRenderers(shellKind, context);
        const std::unique_ptr<head::Shell> shell = buildShell(shellKind, queries.theme);

        model::EventQueue events;
        const std::unique_ptr<head::EventBus> bus = buildEventBus(context);
        const std::unique_ptr<model::EventRouter> router =
            buildEventRouter(intents, scenarios, context, queries);

        head::EventPipeline pipeline{context, *bus, events, *router, scenarios};

        shell->run(context, renderers, pipeline, [&application] {
            return application.isFinished();
        });
    }
}  // namespace cpp_warships::application
