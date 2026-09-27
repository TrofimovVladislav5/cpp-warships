#include <application/head/input/BattleEventHandlers.h>

#include <algorithm>
#include <memory>
#include <optional>
#include <utility>

#include <application/head/intents/FireAtIntent.h>
#include <application/head/intents/ShowScreenIntent.h>
#include <application/head/intents/UseSkillIntent.h>
#include <application/head/screens/ScreenKind.h>

namespace cpp_warships::head {
    namespace {
        constexpr int LOG_SCROLL_STEP = 3;

        bool isPlayerFree(const flow::Match& match) {
            return match.phase() == flow::MatchPhase::Battle && match.isPlayerTurn();
        }
    } // namespace

    FireEventHandler::FireEventHandler(IntentSink intentSink, BattleState& state, MatchQuery match)
        : intentSink_(std::move(intentSink))
        , state_(state)
        , match_(std::move(match)) {}

    bool FireEventHandler::isHandled(const InputEvent& input) const {
        return input.stroke.key == Key::Enter && isPlayerFree(match_());
    }

    void FireEventHandler::handleEvent(const InputEvent&) {
        state_.logScroll = 0;
        intentSink_(std::make_shared<FireAtIntent>(state_.target));
    }

    UseSkillEventHandler::UseSkillEventHandler(
            IntentSink intentSink,
            BattleState& state,
            MatchQuery match
    )
        : intentSink_(std::move(intentSink))
        , state_(state)
        , match_(std::move(match)) {}

    bool UseSkillEventHandler::isHandled(const InputEvent& input) const {
        const bool isRelatedCharacter = isCharacter(input.stroke, "k");
        const bool isSkillsFull = !match_().skills().isEmpty();

        return isRelatedCharacter && isPlayerFree(match_()) && isSkillsFull;
    }

    void UseSkillEventHandler::handleEvent(const InputEvent&) {
        state_.logScroll = 0;

        if (match_().nextSkillNeedsTarget()) {
            intentSink_(std::make_shared<UseSkillIntent>(std::optional{state_.target}));
        } else {
            intentSink_(std::make_shared<UseSkillIntent>(std::nullopt));
        }
    }

    BattleMouseEventHandler::BattleMouseEventHandler(
            IntentSink intentSink,
            BattleState& state,
            MatchQuery match
    )
        : intentSink_(std::move(intentSink))
        , state_(state)
        , match_(std::move(match)) {}

    bool BattleMouseEventHandler::isHandled(const InputEvent& input) const {
        return input.region == InputRegion::EnemyWaters && input.cell.has_value();
    }

    void BattleMouseEventHandler::handleEvent(const InputEvent& input) {
        state_.target = *input.cell;

        const Keystroke& mouse = input.stroke;
        const bool isFiring = mouse.button == PointerButton::Left && mouse.isPressed;
        if (isFiring && isPlayerFree(match_())) {
            state_.logScroll = 0;
            intentSink_(std::make_shared<FireAtIntent>(*input.cell));
        }
    }

    ScrollLogEventHandler::ScrollLogEventHandler(BattleState& state, const BattleJournal& journal)
        : state_(state)
        , journal_(journal) {}

    bool ScrollLogEventHandler::isHandled(const InputEvent& input) const {
        if (input.stroke.key == Key::PageUp || input.stroke.key == Key::PageDown) {
            return true;
        }

        return input.region == InputRegion::Log && isWheelRolled(input.stroke);
    }

    void ScrollLogEventHandler::handleEvent(const InputEvent& input) {
        const bool isBackwards =
                input.stroke.key == Key::PageUp ||
                (isPointer(input.stroke) && input.stroke.button == PointerButton::WheelUp);
        const int newStep = isBackwards ? LOG_SCROLL_STEP : -LOG_SCROLL_STEP;
        const int furthestLog = furthestLogScroll(static_cast<int>(journal_.entries().size()));

        state_.logScroll = std::clamp(state_.logScroll + newStep, 0, furthestLog);
    }

    LeaveBattleEventHandler::LeaveBattleEventHandler(IntentSink intentSink)
        : intentSink_(std::move(intentSink)) {}

    bool LeaveBattleEventHandler::isHandled(const InputEvent& input) const {
        return input.stroke.key == Key::Escape;
    }

    void LeaveBattleEventHandler::handleEvent(const InputEvent&) {
        intentSink_(std::make_shared<ShowScreenIntent>(ScreenKind::Menu));
    }
} // namespace cpp_warships::head
