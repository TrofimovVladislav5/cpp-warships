#include <game_tui/intents/BeginBattleIntent.h>

#include <game_tui/intents/IntentContext.h>
#include <game_tui/screens/ScreenKind.h>
#include <game_tui/screens/ScreenNavigator.h>
#include <game_tui/session/Application.h>

namespace cpp_warships::game_tui {
    void BeginBattleIntent::applyTo(const IntentContext& context) const {
        if (context.session.beginBattle()) {
            context.navigator.showScreen(ScreenKind::Battle);
        }
    }
} // namespace cpp_warships::game_tui
