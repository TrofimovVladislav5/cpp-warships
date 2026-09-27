#include <application/head/input/handlers/PlacementHandlers.h>

#include <utility>
#include <variant>

#include <application/model/scenarios/SequenceScenario.h>

namespace cpp_warships::head {
    PlaceShipHandler::PlaceShipHandler(HandlerParts parts) noexcept
        : parts_(parts) {}

    bool PlaceShipHandler::isHandled(const model::GameEvent& event) const {
        return model::isKind<model::ShipPlacementRequested>(event);
    }

    void PlaceShipHandler::handleEvent(const model::GameEvent& event) {
        const auto& asked = std::get<model::ShipPlacementRequested>(event);
        parts_.scenarios.submit(
                model::scenarioOf(
                        "laying a ship",
                        parts_.intents.placeShip(asked.origin, asked.direction, asked.length)
                )
        );
    }

    RemoveShipHandler::RemoveShipHandler(HandlerParts parts) noexcept
        : parts_(parts) {}

    bool RemoveShipHandler::isHandled(const model::GameEvent& event) const {
        return model::isKind<model::ShipRemovalRequested>(event);
    }

    void RemoveShipHandler::handleEvent(const model::GameEvent& event) {
        const auto& asked = std::get<model::ShipRemovalRequested>(event);
        parts_.scenarios.submit(
                model::scenarioOf(
                        "taking a ship back",
                        parts_.intents.removeShip(asked.coordinate)
                )
        );
    }

    ShuffleFleetHandler::ShuffleFleetHandler(HandlerParts parts) noexcept
        : parts_(parts) {}

    bool ShuffleFleetHandler::isHandled(const model::GameEvent& event) const {
        return model::isKind<model::FleetShuffleRequested>(event);
    }

    void ShuffleFleetHandler::handleEvent(const model::GameEvent&) {
        parts_.scenarios.submit(
                model::scenarioOf("shuffling the fleet", parts_.intents.shuffleFleet())
        );
    }

    BeginBattleHandler::BeginBattleHandler(HandlerParts parts, MatchQuery match) noexcept
        : parts_(parts)
        , match_(std::move(match)) {}

    bool BeginBattleHandler::isHandled(const model::GameEvent& event) const {
        return model::isKind<model::BattleBeginRequested>(event) &&
               match_().playerPlacementPlan().isComplete();
    }

    void BeginBattleHandler::handleEvent(const model::GameEvent&) {
        parts_.scenarios.submit(
                model::scenarioOf("opening fire", parts_.intents.beginBattle())
        );
    }
} // namespace cpp_warships::head
