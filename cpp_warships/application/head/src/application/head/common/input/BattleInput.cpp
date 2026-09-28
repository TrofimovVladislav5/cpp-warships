#include <application/core/Board.h>
#include <application/head/common/input/BattleInput.h>

#include <algorithm>
#include <utility>

namespace cpp_warships::head {
    namespace {
        constexpr int LOG_SCROLL_STEP = 3;

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
    }  // namespace

    BattleInput::BattleInput(PresentationContext& context) noexcept : context_(context) {
    }

    std::optional<model::GameEvent> BattleInput::interpret(const Keystroke& stroke) {
        if (isPointer(stroke)) {
            return interpretPointer(stroke);
        }
        if (isArrow(stroke)) {
            moveAim(stroke);
            return std::nullopt;
        }
        if (stroke.key == Key::PageUp) {
            scrollLog(LOG_SCROLL_STEP);
            return std::nullopt;
        }
        if (stroke.key == Key::PageDown) {
            scrollLog(-LOG_SCROLL_STEP);
            return std::nullopt;
        }
        if (stroke.key == Key::Enter) {
            context_.state().battle.logScroll = 0;
            return model::ShotRequested{.target = context_.state().battle.target};
        }
        if (isCharacter(stroke, "k")) {
            context_.state().battle.logScroll = 0;
            return model::SkillUseRequested{.aim = context_.state().battle.target};
        }
        if (stroke.key == Key::Escape) {
            return model::MenuReturnRequested{};
        }

        return std::nullopt;
    }

    std::optional<model::GameEvent> BattleInput::interpretPointer(const Keystroke& stroke) {
        const ScreenRegion region = context_.geometry().regionAt(stroke.pointerX, stroke.pointerY);

        if (region == ScreenRegion::Log && isWheelRolled(stroke)) {
            scrollLog(stroke.button == PointerButton::WheelUp ? LOG_SCROLL_STEP : -LOG_SCROLL_STEP);
            return std::nullopt;
        }

        const std::optional<core::Coordinate> cell =
            context_.geometry().cellAt(ScreenRegion::EnemyWaters, stroke.pointerX, stroke.pointerY);
        if (!cell.has_value()) {
            return std::nullopt;
        }

        context_.state().battle.target = *cell;

        if (stroke.button == PointerButton::Left && stroke.isPressed) {
            context_.state().battle.logScroll = 0;
            return model::ShotRequested{.target = *cell};
        }

        return std::nullopt;
    }

    void BattleInput::moveAim(const Keystroke& stroke) {
        const core::Board& board = context_.game().match().computerBoard();
        const core::Coordinate step = stepOf(stroke);

        context_.state().battle.target.x =
            std::clamp(context_.state().battle.target.x + step.x, 0, board.width() - 1);
        context_.state().battle.target.y =
            std::clamp(context_.state().battle.target.y + step.y, 0, board.height() - 1);
    }

    void BattleInput::scrollLog(const int step) {
        const int furthest =
            furthestLogScroll(static_cast<int>(context_.game().journal().entries().size()));
        context_.state().battle.logScroll =
            std::clamp(context_.state().battle.logScroll + step, 0, furthest);
    }
}  // namespace cpp_warships::head
