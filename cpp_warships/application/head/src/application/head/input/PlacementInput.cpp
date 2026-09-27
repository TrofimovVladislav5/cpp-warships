#include <application/head/input/PlacementInput.h>

#include <algorithm>
#include <utility>

#include <application/core/Board.h>

namespace cpp_warships::head {
    namespace {
        [[nodiscard]] core::Coordinate stepOf(const Keystroke& stroke) {
            if (stroke.key == Key::ArrowLeft) {
                return {-1, 0};
            }
            if (stroke.key == Key::ArrowRight) {
                return {1, 0};
            }
            if (stroke.key == Key::ArrowUp) {
                return {0, -1};
            }

            return {0, 1};
        }

        [[nodiscard]] bool isArrow(const Keystroke& stroke) {
            return stroke.key == Key::ArrowLeft || stroke.key == Key::ArrowRight ||
                   stroke.key == Key::ArrowUp || stroke.key == Key::ArrowDown;
        }

        [[nodiscard]] core::Direction turnedFrom(const core::Direction direction) {
            return direction == core::Direction::Horizontal ? core::Direction::Vertical
                                                            : core::Direction::Horizontal;
        }
    } // namespace

    PlacementInput::PlacementInput(PresentationContext& context) noexcept
        : context_(context) {}

    std::optional<model::GameEvent> PlacementInput::interpret(const Keystroke& stroke) {
        if (isPointer(stroke)) {
            return interpretPointer(stroke);
        }
        if (isArrow(stroke)) {
            moveCursor(stroke);
            return std::nullopt;
        }
        if (isCharacter(stroke, "r")) {
            turnShip();
            return std::nullopt;
        }
        if (stroke.key == Key::Tab) {
            pickNextShipLength();
            return std::nullopt;
        }
        if (stroke.key == Key::Enter) {
            return layShipInHand();
        }
        if (stroke.key == Key::Backspace || stroke.key == Key::Delete) {
            return model::ShipRemovalRequested{.coordinate = context_.state().placement.cursor};
        }
        if (isCharacter(stroke, "s")) {
            return model::FleetShuffleRequested{};
        }
        if (isCharacter(stroke, "b")) {
            return model::BattleBeginRequested{};
        }
        if (stroke.key == Key::SaveKey) {
            return model::MatchSaveRequested{};
        }
        if (stroke.key == Key::Escape) {
            return model::MenuReturnRequested{};
        }

        return std::nullopt;
    }

    std::optional<model::GameEvent> PlacementInput::interpretPointer(const Keystroke& stroke) {
        const std::optional<core::Coordinate> cell =
                context_.geometry().cellAt(ScreenRegion::OwnWaters, stroke.pointerX, stroke.pointerY);
        if (!cell.has_value()) {
            return std::nullopt;
        }

        context_.state().placement.cursor = *cell;

        if (isWheelRolled(stroke)) {
            pickNextShipLength();
            return std::nullopt;
        }
        if (!stroke.isPressed) {
            return std::nullopt;
        }
        if (stroke.button == PointerButton::Left) {
            return layShipInHand();
        }
        if (stroke.button == PointerButton::Right) {
            return model::ShipRemovalRequested{.coordinate = *cell};
        }

        return std::nullopt;
    }

    std::optional<model::GameEvent> PlacementInput::layShipInHand() const {
        const int lengthInHand = shipLengthInHand(context_.game().match().playerPlacementPlan(), context_.state().placement);
        if (lengthInHand <= 0) {
            return std::nullopt;
        }

        return model::ShipPlacementRequested{
                .origin = context_.state().placement.cursor,
                .direction = context_.state().placement.direction,
                .length = lengthInHand
        };
    }

    void PlacementInput::moveCursor(const Keystroke& stroke) {
        const core::Board& board = context_.game().match().playerBoard();
        const core::Coordinate step = stepOf(stroke);

        context_.state().placement.cursor.x = std::clamp(context_.state().placement.cursor.x + step.x, 0, board.width() - 1);
        context_.state().placement.cursor.y = std::clamp(context_.state().placement.cursor.y + step.y, 0, board.height() - 1);
    }

    void PlacementInput::turnShip() {
        context_.state().placement.direction = turnedFrom(context_.state().placement.direction);
    }

    void PlacementInput::pickNextShipLength() {
        const flow::PlacementPlan& plan = context_.game().match().playerPlacementPlan();
        context_.state().placement.preferredShipLength = nextShipLength(plan, shipLengthInHand(plan, context_.state().placement));
    }
} // namespace cpp_warships::head
