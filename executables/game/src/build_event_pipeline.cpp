#include <build_event_pipeline.h>

#include <application/head/input/BattleInput.h>
#include <application/head/input/MenuInput.h>
#include <application/head/input/PlacementInput.h>
#include <application/head/input/handlers/BattleHandlers.h>
#include <application/head/input/handlers/PlacementHandlers.h>
#include <application/head/input/handlers/SessionHandlers.h>

namespace cpp_warships::application {
    std::unique_ptr<head::EventBus> buildEventBus(head::PresentationContext& context) {
        auto bus = std::make_unique<head::EventBus>();

        bus->readScreenWith(
                head::ScreenKind::Menu,
                std::make_unique<head::MenuInput>(context)
        );
        bus->readScreenWith(
                head::ScreenKind::Placement,
                std::make_unique<head::PlacementInput>(context)
        );
        bus->readScreenWith(
                head::ScreenKind::Battle,
                std::make_unique<head::BattleInput>(context)
        );

        return bus;
    }

    std::unique_ptr<model::EventRouter> buildEventRouter(
            const model::IntentFactory& intents,
            model::ScenarioQueue& scenarios,
            head::PresentationContext& context,
            const SessionQueries& queries
    ) {
        auto router = std::make_unique<model::EventRouter>();
        const head::HandlerParts parts{.intents = intents, .scenarios = scenarios};

        // Saving and quitting work wherever the player happens to be.
        router->subscribe(
                model::EventScope::Always,
                std::make_shared<head::SaveMatchHandler>(parts)
        );
        router->subscribe(
                model::EventScope::Always,
                std::make_shared<head::QuitHandler>(parts)
        );
        router->subscribe(
                model::EventScope::Always,
                std::make_shared<head::ReturnToMenuHandler>(context.state())
        );

        router->subscribe(
                model::EventScope::Menu,
                std::make_shared<head::StartMatchHandler>(parts, context.state())
        );
        router->subscribe(
                model::EventScope::Menu,
                std::make_shared<head::ResumeMatchHandler>(context.state(), queries.hasMatch)
        );
        router->subscribe(
                model::EventScope::Menu,
                std::make_shared<head::LoadMatchHandler>(parts, context.state(), queries.hasSavedMatch)
        );

        router->subscribe(
                model::EventScope::Placement,
                std::make_shared<head::PlaceShipHandler>(parts)
        );
        router->subscribe(
                model::EventScope::Placement,
                std::make_shared<head::RemoveShipHandler>(parts)
        );
        router->subscribe(
                model::EventScope::Placement,
                std::make_shared<head::ShuffleFleetHandler>(parts)
        );
        router->subscribe(
                model::EventScope::Placement,
                std::make_shared<head::BeginBattleHandler>(parts, queries.match)
        );

        router->subscribe(
                model::EventScope::Battle,
                std::make_shared<head::FireHandler>(parts, queries.match)
        );
        router->subscribe(
                model::EventScope::Battle,
                std::make_shared<head::UseSkillHandler>(parts, queries.match)
        );

        return router;
    }
} // namespace cpp_warships::application
