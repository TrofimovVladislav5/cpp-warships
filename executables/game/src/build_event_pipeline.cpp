#include <application/head/common/PresentationContext.h>
#include <application/head/common/input/BattleInput.h>
#include <application/head/common/input/MenuInput.h>
#include <application/head/common/input/PlacementInput.h>
#include <application/head/common/input/SaveBrowserInput.h>
#include <application/head/common/input/SaveNamingInput.h>
#include <application/head/common/input/handlers/BattleHandlers.h>
#include <application/head/common/input/handlers/PlacementHandlers.h>
#include <application/head/common/input/handlers/SessionHandlers.h>
#include <application/model/intents/IntentFactory.h>
#include <application/model/scenarios/ScenarioQueue.h>
#include <build_event_pipeline.h>

namespace cpp_warships::application {
    std::unique_ptr<head::common::input::EventBus> buildEventBus(
        head::common::PresentationContext& context
    ) {
        auto bus = std::make_unique<head::common::input::EventBus>();

        bus->readScreenWith(
            head::common::ScreenKind::Menu,
            std::make_unique<head::common::input::MenuInput>(context)
        );
        bus->readScreenWith(
            head::common::ScreenKind::Placement,
            std::make_unique<head::common::input::PlacementInput>(context)
        );
        bus->readScreenWith(
            head::common::ScreenKind::Battle,
            std::make_unique<head::common::input::BattleInput>(context)
        );
        bus->readScreenWith(
            head::common::ScreenKind::Saves,
            std::make_unique<head::common::input::SaveBrowserInput>(context)
        );
        bus->readScreenWith(
            head::common::ScreenKind::SaveNaming,
            std::make_unique<head::common::input::SaveNamingInput>(context)
        );

        return bus;
    }

    std::unique_ptr<model::events::EventRouter> buildEventRouter(
        const model::intents::IntentFactory& intents,
        model::scenarios::ScenarioQueue& scenarios,
        head::common::PresentationContext& context,
        const SessionQueries& queries
    ) {
        auto router = std::make_unique<model::events::EventRouter>();
        const head::common::input::handlers::HandlerParts parts{
            .intents = intents,
            .scenarios = scenarios
        };

        router->subscribe(
            model::events::EventScope::Always,
            std::make_shared<head::common::input::handlers::QuitHandler>(parts)
        );
        router->subscribe(
            model::events::EventScope::Always,
            std::make_shared<head::common::input::handlers::ReturnToMenuHandler>(context.state())
        );

        router->subscribe(
            model::events::EventScope::SaveNaming,
            std::make_shared<head::common::input::handlers::SaveAndQuitHandler>(
                parts,
                context.state()
            )
        );

        router->subscribe(
            model::events::EventScope::Menu,
            std::make_shared<head::common::input::handlers::StartMatchHandler>(
                parts,
                context.state()
            )
        );
        router->subscribe(
            model::events::EventScope::Menu,
            std::make_shared<head::common::input::handlers::ResumeMatchHandler>(
                context.state(),
                queries.hasMatch
            )
        );
        router->subscribe(
            model::events::EventScope::Menu,
            std::make_shared<head::common::input::handlers::OpenSaveNamingHandler>(
                context.state(),
                queries.hasMatch,
                queries.nameInPlay
            )
        );
        router->subscribe(
            model::events::EventScope::Menu,
            std::make_shared<head::common::input::handlers::OpenSaveBrowserHandler>(
                context.state(),
                queries.hasMatch,
                queries.hasSavedMatch
            )
        );
        router->subscribe(
            model::events::EventScope::Saves,
            std::make_shared<head::common::input::handlers::LoadMatchHandler>(
                parts,
                context.state()
            )
        );
        router->subscribe(
            model::events::EventScope::Saves,
            std::make_shared<head::common::input::handlers::DeleteSaveHandler>(
                parts,
                context.state()
            )
        );

        router->subscribe(
            model::events::EventScope::Placement,
            std::make_shared<head::common::input::handlers::PlaceShipHandler>(parts)
        );
        router->subscribe(
            model::events::EventScope::Placement,
            std::make_shared<head::common::input::handlers::RemoveShipHandler>(parts)
        );
        router->subscribe(
            model::events::EventScope::Placement,
            std::make_shared<head::common::input::handlers::ShuffleFleetHandler>(parts)
        );
        router->subscribe(
            model::events::EventScope::Placement,
            std::make_shared<head::common::input::handlers::BeginBattleHandler>(
                parts,
                queries.match
            )
        );

        router->subscribe(
            model::events::EventScope::Battle,
            std::make_shared<head::common::input::handlers::FireHandler>(parts, queries.match)
        );
        router->subscribe(
            model::events::EventScope::Battle,
            std::make_shared<head::common::input::handlers::UseSkillHandler>(parts, queries.match)
        );

        return router;
    }
}  // namespace cpp_warships::application
