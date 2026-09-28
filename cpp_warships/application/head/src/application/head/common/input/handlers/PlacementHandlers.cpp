#include <application/head/common/input/handlers/PlacementHandlers.h>
#include <application/model/intents/IntentFactory.h>
#include <application/model/scenarios/ScenarioQueue.h>
#include <application/model/scenarios/SequenceScenario.h>

#include <utility>
#include <variant>

namespace cpp_warships::head::common::input::handlers {
    PlaceShipHandler::PlaceShipHandler(HandlerParts parts) noexcept
        : parts_(parts) {}

    bool PlaceShipHandler::isHandled(const model::events::GameEvent& event) const {
        return model::events::isKind<model::events::ShipPlacementRequested>(event);
    }

    void PlaceShipHandler::handleEvent(const model::events::GameEvent& event) {
        const auto& asked = std::get<model::events::ShipPlacementRequested>(event);
        parts_.scenarios.submit(
            model::scenarios::scenarioOf(
                "laying a ship",
                parts_.intents.placeShip(asked.origin, asked.direction, asked.length)
            )
        );
    }

    RemoveShipHandler::RemoveShipHandler(HandlerParts parts) noexcept
        : parts_(parts) {}

    bool RemoveShipHandler::isHandled(const model::events::GameEvent& event) const {
        return model::events::isKind<model::events::ShipRemovalRequested>(event);
    }

    void RemoveShipHandler::handleEvent(const model::events::GameEvent& event) {
        const auto& asked = std::get<model::events::ShipRemovalRequested>(event);
        parts_.scenarios.submit(
            model::scenarios::scenarioOf(
                "taking a ship back",
                parts_.intents.removeShip(asked.coordinate)
            )
        );
    }

    ShuffleFleetHandler::ShuffleFleetHandler(HandlerParts parts) noexcept
        : parts_(parts) {}

    bool ShuffleFleetHandler::isHandled(const model::events::GameEvent& event) const {
        return model::events::isKind<model::events::FleetShuffleRequested>(event);
    }

    void ShuffleFleetHandler::handleEvent(const model::events::GameEvent&) {
        parts_.scenarios.submit(
            model::scenarios::scenarioOf("shuffling the fleet", parts_.intents.shuffleFleet())
        );
    }

    BeginBattleHandler::BeginBattleHandler(HandlerParts parts, MatchQuery match) noexcept
        : parts_(parts)
        , match_(std::move(match)) {}

    bool BeginBattleHandler::isHandled(const model::events::GameEvent& event) const {
        return model::events::isKind<model::events::BattleBeginRequested>(event) &&
               match_().playerPlacementPlan().isComplete();
    }

    void BeginBattleHandler::handleEvent(const model::events::GameEvent&) {
        parts_.scenarios.submit(
            model::scenarios::scenarioOf("opening fire", parts_.intents.beginBattle())
        );
    }
}  // namespace cpp_warships::head::common::input::handlers
