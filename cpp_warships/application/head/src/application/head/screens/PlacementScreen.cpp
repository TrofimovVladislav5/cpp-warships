#include <application/head/screens/PlacementScreen.h>

#include <memory>
#include <utility>

#include <application/head/input/MoveCursorEventHandler.h>
#include <application/head/input/PlacementEventHandlers.h>
#include <application/head/input/SaveMatchEventHandler.h>

namespace cpp_warships::head {
    PlacementScreen::PlacementScreen(
            IntentSink intentSink,
            const Theme& theme,
            MatchQuery match,
            const PlacementViewFactory& makeView
    )
        : match_(std::move(match))
        , view_(makeView(theme, match_, state_)) {
        const BoardQuery ownBoard = [this]() -> const core::Board& {
            return match_().playerBoard();
        };

        eventRouter_.add(
                std::make_shared<LayShipWithMouseEventHandler>(intentSink, state_, match_)
        );
        eventRouter_.add(std::make_shared<TakeBackShipWithMouseEventHandler>(intentSink, state_));
        eventRouter_.add(std::make_shared<PickShipWithWheelEventHandler>(state_, match_));
        eventRouter_.add(std::make_shared<TakeAimWithMouseEventHandler>(state_));
        eventRouter_.add(std::make_shared<MoveCursorEventHandler>(state_.cursor, ownBoard));
        eventRouter_.add(std::make_shared<RotateShipEventHandler>(state_));
        eventRouter_.add(std::make_shared<CycleShipLengthEventHandler>(state_, match_));
        eventRouter_.add(std::make_shared<PlaceShipEventHandler>(intentSink, state_, match_));
        eventRouter_.add(std::make_shared<RemoveShipEventHandler>(intentSink, state_));
        eventRouter_.add(std::make_shared<ShuffleFleetEventHandler>(intentSink));
        eventRouter_.add(std::make_shared<BeginBattleEventHandler>(intentSink, match_));
        eventRouter_.add(std::make_shared<SaveMatchEventHandler>(intentSink));
        eventRouter_.add(std::make_shared<LeavePlacementEventHandler>(intentSink));
    }

    ScreenKind PlacementScreen::kind() const {
        return ScreenKind::Placement;
    }

    GameView& PlacementScreen::view() {
        return *view_;
    }

    bool PlacementScreen::handleEvent(const Keystroke& stroke) {
        return eventRouter_.dispatch(view_->interpret(stroke));
    }
} // namespace cpp_warships::head
