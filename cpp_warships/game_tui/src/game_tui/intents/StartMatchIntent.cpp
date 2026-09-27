#include <game_tui/intents/StartMatchIntent.h>

#include <game_tui/intents/IntentContext.h>
#include <game_tui/screens/ScreenKind.h>
#include <game_tui/screens/ScreenNavigator.h>
#include <game_tui/session/Application.h>

namespace cpp_warships::game_tui {
    StartMatchIntent::StartMatchIntent(int boardSize)
        : boardSize_(boardSize) {}

    void StartMatchIntent::applyTo(const IntentContext& context) const {
        context.session.startNewMatch(boardSize_);
        context.navigator.showScreen(ScreenKind::Placement);
    }
} // namespace cpp_warships::game_tui
