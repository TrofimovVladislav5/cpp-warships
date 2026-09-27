#include <game_tui/screens/MenuScreen.h>

#include <memory>
#include <utility>

#include <game_tui/input/MenuEventHandlers.h>

namespace cpp_warships::game_tui {

    MenuScreen::MenuScreen(
            IntentSink intentSink,
            const Theme& theme,
            MatchInProgressQuery hasMatch,
            SavedMatchQuery hasSavedMatch,
            const MenuViewFactory& makeView
    )
        : view_(makeView(theme, state_, hasMatch, hasSavedMatch)) {
        eventRouter_.add(std::make_shared<BoardSizeEventHandler>(state_));
        eventRouter_.add(std::make_shared<StartMatchEventHandler>(intentSink, state_));
        eventRouter_.add(
                std::make_shared<ResumeMatchEventHandler>(intentSink, std::move(hasMatch))
        );
        eventRouter_.add(
                std::make_shared<LoadMatchEventHandler>(intentSink, std::move(hasSavedMatch))
        );
        eventRouter_.add(std::make_shared<CycleThemeEventHandler>(intentSink, state_));
        eventRouter_.add(std::make_shared<QuitEventHandler>(intentSink));
    }

    ScreenKind MenuScreen::kind() const {
        return ScreenKind::Menu;
    }

    GameView& MenuScreen::view() {
        return *view_;
    }

    bool MenuScreen::handleEvent(const Keystroke& stroke) {
        return eventRouter_.dispatch(view_->interpret(stroke));
    }
} // namespace cpp_warships::game_tui
