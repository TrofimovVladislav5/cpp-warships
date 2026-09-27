#include <game_tui/intents/ShowScreenIntent.h>

#include <game_tui/intents/IntentContext.h>
#include <game_tui/screens/ScreenNavigator.h>
#include <game_tui/session/Application.h>

namespace cpp_warships::game_tui {
    ShowScreenIntent::ShowScreenIntent(ScreenKind screen)
        : screen_(screen) {}

    void ShowScreenIntent::applyTo(const IntentContext& context) const {
        context.navigator.showScreen(screen_);
    }
} // namespace cpp_warships::game_tui
