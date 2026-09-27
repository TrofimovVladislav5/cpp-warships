#include <game_tui/screens/BattleScreen.h>

#include <memory>
#include <utility>

#include <game_tui/input/BattleEventHandlers.h>
#include <game_tui/input/MoveCursorEventHandler.h>
#include <game_tui/input/SaveMatchEventHandler.h>

namespace cpp_warships::game_tui {
    BattleScreen::BattleScreen(
            IntentSink intentSink,
            const Theme& theme,
            MatchQuery match,
            const BattleJournal& journal,
            const BattleViewFactory& makeView
    )
        : match_(std::move(match))
        , view_(makeView(theme, match_, journal, state_)) {
        const BoardQuery enemyWaters = [this]() -> const game_core::Board& {
            return match_().computerBoard();
        };

        eventRouter_.add(std::make_shared<ScrollLogEventHandler>(state_, journal));
        eventRouter_.add(std::make_shared<BattleMouseEventHandler>(intentSink, state_, match_));
        eventRouter_.add(std::make_shared<MoveCursorEventHandler>(state_.target, enemyWaters));
        eventRouter_.add(std::make_shared<FireEventHandler>(intentSink, state_, match_));
        eventRouter_.add(std::make_shared<UseSkillEventHandler>(intentSink, state_, match_));
        eventRouter_.add(std::make_shared<SaveMatchEventHandler>(intentSink));
        eventRouter_.add(std::make_shared<LeaveBattleEventHandler>(intentSink));
    }

    ScreenKind BattleScreen::kind() const {
        return ScreenKind::Battle;
    }

    GameView& BattleScreen::view() {
        return *view_;
    }

    bool BattleScreen::handleEvent(const Keystroke& stroke) {
        return eventRouter_.dispatch(view_->interpret(stroke));
    }
} // namespace cpp_warships::game_tui
