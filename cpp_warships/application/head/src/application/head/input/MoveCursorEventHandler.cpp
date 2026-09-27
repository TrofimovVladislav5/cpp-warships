#include <application/head/input/MoveCursorEventHandler.h>

#include <algorithm>
#include <utility>

#include <application/core/Board.h>

namespace cpp_warships::head {
    namespace {
        core::Coordinate stepOf(const Keystroke& stroke) {
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
    } // namespace

    MoveCursorEventHandler::MoveCursorEventHandler(core::Coordinate& cursor, BoardQuery board)
        : cursor_(cursor)
        , board_(std::move(board)) {}

    bool MoveCursorEventHandler::isHandled(const InputEvent& input) const {
        return input.stroke.key == Key::ArrowLeft || input.stroke.key == Key::ArrowRight ||
               input.stroke.key == Key::ArrowUp || input.stroke.key == Key::ArrowDown;
    }

    void MoveCursorEventHandler::handleEvent(const InputEvent& input) {
        const core::Board& board = board_();
        const core::Coordinate step = stepOf(input.stroke);

        cursor_.x = std::clamp(cursor_.x + step.x, 0, board.width() - 1);
        cursor_.y = std::clamp(cursor_.y + step.y, 0, board.height() - 1);
    }
} // namespace cpp_warships::head
