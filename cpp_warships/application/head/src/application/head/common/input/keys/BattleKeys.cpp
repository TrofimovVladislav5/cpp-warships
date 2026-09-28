#include <application/head/common/PresentationContext.h>
#include <application/head/common/input/Keystroke.h>
#include <application/head/common/input/keys/BattleKeys.h>

#include <algorithm>

namespace cpp_warships::head::common::input::keys {
    namespace {
        constexpr int LOG_SCROLL_STEP = 3;
    }  // namespace

    void scrollLog(PresentationContext& context, const int step) {
        const int furthest =
            state::furthestLogScroll(static_cast<int>(context.game().journal().entries().size()));
        int& scrolled = context.state().battle.logScroll;

        scrolled = std::clamp(scrolled + step, 0, furthest);
    }

    BattleKey::BattleKey(PresentationContext& context) noexcept
        : context_(context) {}

    std::optional<core::Coordinate> BattleKey::cellUnderPointer(const Keystroke& stroke) const {
        if (!isPointer(stroke)) {
            return std::nullopt;
        }

        return context_.geometry()
            .cellAt(ScreenRegion::EnemyWaters, stroke.pointerX, stroke.pointerY);
    }

    core::Coordinate& MoveBattleAimKey::cursor() {
        return context_.state().battle.target;
    }

    const core::Board& MoveBattleAimKey::board() const {
        return context_.game().match().computerBoard();
    }

    bool ScrollLogBackKey::matches(const Keystroke& stroke) const {
        return stroke.key == Key::PageUp;
    }

    std::optional<model::events::GameEvent> ScrollLogBackKey::interpret(const Keystroke&) {
        scrollLog(context_, LOG_SCROLL_STEP);
        return std::nullopt;
    }

    bool ScrollLogForwardKey::matches(const Keystroke& stroke) const {
        return stroke.key == Key::PageDown;
    }

    std::optional<model::events::GameEvent> ScrollLogForwardKey::interpret(const Keystroke&) {
        scrollLog(context_, -LOG_SCROLL_STEP);
        return std::nullopt;
    }

    bool FireKey::matches(const Keystroke& stroke) const {
        return stroke.key == Key::Enter;
    }

    std::optional<model::events::GameEvent> FireKey::interpret(const Keystroke&) {
        context_.state().battle.logScroll = 0;
        return model::events::ShotRequested{.target = context_.state().battle.target};
    }

    bool UseSkillKey::matches(const Keystroke& stroke) const {
        return isCharacter(stroke, "k");
    }

    std::optional<model::events::GameEvent> UseSkillKey::interpret(const Keystroke&) {
        context_.state().battle.logScroll = 0;
        return model::events::SkillUseRequested{.aim = context_.state().battle.target};
    }

    bool ScrollLogWithWheelKey::matches(const Keystroke& stroke) const {
        if (!isPointer(stroke) || !isWheelRolled(stroke)) {
            return false;
        }

        return context_.geometry().regionAt(stroke.pointerX, stroke.pointerY) == ScreenRegion::Log;
    }

    std::optional<model::events::GameEvent> ScrollLogWithWheelKey::interpret(
        const Keystroke& stroke
    ) {
        const int step =
            stroke.button == PointerButton::WheelUp ? LOG_SCROLL_STEP : -LOG_SCROLL_STEP;
        scrollLog(context_, step);

        return std::nullopt;
    }

    bool FireWithPointerKey::matches(const Keystroke& stroke) const {
        return cellUnderPointer(stroke).has_value() && stroke.isPressed &&
               stroke.button == PointerButton::Left;
    }

    std::optional<model::events::GameEvent> FireWithPointerKey::interpret(const Keystroke& stroke) {
        const core::Coordinate cell = *cellUnderPointer(stroke);
        context_.state().battle.target = cell;
        context_.state().battle.logScroll = 0;

        return model::events::ShotRequested{.target = cell};
    }

    bool AimWithPointerAtEnemyKey::matches(const Keystroke& stroke) const {
        return cellUnderPointer(stroke).has_value();
    }

    std::optional<model::events::GameEvent> AimWithPointerAtEnemyKey::interpret(
        const Keystroke& stroke
    ) {
        context_.state().battle.target = *cellUnderPointer(stroke);
        return std::nullopt;
    }
}  // namespace cpp_warships::head::common::input::keys
