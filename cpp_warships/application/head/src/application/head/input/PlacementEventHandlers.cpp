#include <application/head/input/PlacementEventHandlers.h>

#include <memory>
#include <optional>
#include <string>
#include <utility>

#include <application/head/intents/BeginBattleIntent.h>
#include <application/head/intents/PlaceShipIntent.h>
#include <application/head/intents/RemoveShipIntent.h>
#include <application/head/intents/ShowScreenIntent.h>
#include <application/head/intents/ShuffleFleetIntent.h>
#include <application/head/screens/ScreenKind.h>

namespace cpp_warships::head {
    namespace {
        void layShipInHand(
                const IntentSink& intentSink,
                const PlacementState& state,
                const flow::PlacementPlan& plan
        ) {
            const int lengthInHand = shipLengthInHand(plan, state);
            if (lengthInHand <= 0) {
                return;
            }

            intentSink(
                    std::make_shared<PlaceShipIntent>(state.cursor, state.direction, lengthInHand)
            );
        }

        void takeBackShipAt(const IntentSink& intentSink, core::Coordinate cell) {
            intentSink(std::make_shared<RemoveShipIntent>(cell));
        }

        void pickNextShipLength(PlacementState& state, const flow::PlacementPlan& plan) {
            state.preferredShipLength = nextShipLength(plan, shipLengthInHand(plan, state));
        }

        core::Direction turnedFrom(core::Direction direction) {
            return direction == core::Direction::Horizontal ? core::Direction::Vertical
                                                                 : core::Direction::Horizontal;
        }
    } // namespace

    RotateShipEventHandler::RotateShipEventHandler(PlacementState& state)
        : state_(state) {}

    bool RotateShipEventHandler::isHandled(const InputEvent& input) const {
        return isCharacter(input.stroke, "r");
    }

    void RotateShipEventHandler::handleEvent(const InputEvent&) {
        state_.direction = turnedFrom(state_.direction);
    }

    CycleShipLengthEventHandler::CycleShipLengthEventHandler(
            PlacementState& state,
            MatchQuery match
    )
        : state_(state)
        , match_(std::move(match)) {}

    bool CycleShipLengthEventHandler::isHandled(const InputEvent& input) const {
        return input.stroke.key == Key::Tab;
    }

    void CycleShipLengthEventHandler::handleEvent(const InputEvent&) {
        pickNextShipLength(state_, match_().playerPlacementPlan());
    }

    PlaceShipEventHandler::PlaceShipEventHandler(
            IntentSink intentSink,
            const PlacementState& state,
            MatchQuery match
    )
        : intentSink_(std::move(intentSink))
        , state_(state)
        , match_(std::move(match)) {}

    bool PlaceShipEventHandler::isHandled(const InputEvent& input) const {
        return input.stroke.key == Key::Enter;
    }

    void PlaceShipEventHandler::handleEvent(const InputEvent&) {
        layShipInHand(intentSink_, state_, match_().playerPlacementPlan());
    }

    RemoveShipEventHandler::RemoveShipEventHandler(
            IntentSink intentSink,
            const PlacementState& state
    )
        : intentSink_(std::move(intentSink))
        , state_(state) {}

    bool RemoveShipEventHandler::isHandled(const InputEvent& input) const {
        return input.stroke.key == Key::Backspace || input.stroke.key == Key::Delete;
    }

    void RemoveShipEventHandler::handleEvent(const InputEvent&) {
        takeBackShipAt(intentSink_, state_.cursor);
    }

    ShuffleFleetEventHandler::ShuffleFleetEventHandler(IntentSink intentSink)
        : intentSink_(std::move(intentSink)) {}

    bool ShuffleFleetEventHandler::isHandled(const InputEvent& input) const {
        return isCharacter(input.stroke, "s");
    }

    void ShuffleFleetEventHandler::handleEvent(const InputEvent&) {
        intentSink_(std::make_shared<ShuffleFleetIntent>());
    }

    BeginBattleEventHandler::BeginBattleEventHandler(IntentSink intentSink, MatchQuery match)
        : intentSink_(std::move(intentSink))
        , match_(std::move(match)) {}

    bool BeginBattleEventHandler::isHandled(const InputEvent& input) const {
        const bool isRelatedCharacter = isCharacter(input.stroke, "b");

        return isRelatedCharacter && match_().playerPlacementPlan().isComplete();
    }

    void BeginBattleEventHandler::handleEvent(const InputEvent&) {
        intentSink_(std::make_shared<BeginBattleIntent>());
    }

    GridMouseEventHandler::GridMouseEventHandler(PlacementState& state)
        : state_(state) {}

    bool GridMouseEventHandler::isHandled(const InputEvent& input) const {
        return input.region == InputRegion::OwnWaters && input.cell.has_value() &&
               isPointer(input.stroke) && claims(input.stroke);
    }

    void GridMouseEventHandler::handleEvent(const InputEvent& input) {
        state_.cursor = *input.cell;
        actOn(*input.cell);
    }

    PlacementState& GridMouseEventHandler::state() const noexcept {
        return state_;
    }

    bool TakeAimWithMouseEventHandler::claims(const Keystroke&) const {
        return true;
    }

    void TakeAimWithMouseEventHandler::actOn(core::Coordinate) {}

    LayShipWithMouseEventHandler::LayShipWithMouseEventHandler(
            IntentSink intentSink,
            PlacementState& state,
            MatchQuery match
    )
        : GridMouseEventHandler(state)
        , intentSink_(std::move(intentSink))
        , match_(std::move(match)) {}

    bool LayShipWithMouseEventHandler::claims(const Keystroke& mouse) const {
        return mouse.button == PointerButton::Left && mouse.isPressed;
    }

    void LayShipWithMouseEventHandler::actOn(core::Coordinate) {
        layShipInHand(intentSink_, state(), match_().playerPlacementPlan());
    }

    TakeBackShipWithMouseEventHandler::TakeBackShipWithMouseEventHandler(
            IntentSink intentSink,
            PlacementState& state
    )
        : GridMouseEventHandler(state)
        , intentSink_(std::move(intentSink)) {}

    bool TakeBackShipWithMouseEventHandler::claims(const Keystroke& mouse) const {
        return mouse.button == PointerButton::Right && mouse.isPressed;
    }

    void TakeBackShipWithMouseEventHandler::actOn(core::Coordinate cell) {
        takeBackShipAt(intentSink_, cell);
    }

    PickShipWithWheelEventHandler::PickShipWithWheelEventHandler(
            PlacementState& state,
            MatchQuery match
    )
        : GridMouseEventHandler(state)
        , match_(std::move(match)) {}

    bool PickShipWithWheelEventHandler::claims(const Keystroke& mouse) const {
        return mouse.button == PointerButton::WheelUp || mouse.button == PointerButton::WheelDown;
    }

    void PickShipWithWheelEventHandler::actOn(core::Coordinate) {
        pickNextShipLength(state(), match_().playerPlacementPlan());
    }

    LeavePlacementEventHandler::LeavePlacementEventHandler(IntentSink intentSink)
        : intentSink_(std::move(intentSink)) {}

    bool LeavePlacementEventHandler::isHandled(const InputEvent& input) const {
        return input.stroke.key == Key::Escape;
    }

    void LeavePlacementEventHandler::handleEvent(const InputEvent&) {
        intentSink_(std::make_shared<ShowScreenIntent>(ScreenKind::Menu));
    }
} // namespace cpp_warships::head
