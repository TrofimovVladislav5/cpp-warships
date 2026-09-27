#include <game_tui/intents/LoadMatchIntent.h>

#include <game_tui/intents/IntentContext.h>
#include <game_tui/screens/ScreenKind.h>
#include <game_tui/screens/ScreenNavigator.h>
#include <game_tui/session/Application.h>

namespace cpp_warships::game_tui {
    void LoadMatchIntent::applyTo(const IntentContext& context) const {
        if (!context.session.loadMatch()) {
            return;
        }

        // A save remembers where play had got to, so it reopens on the screen it was left on.
        const bool isLayingOut =
                context.session.match().phase() == game_flow::MatchPhase::Placement;
        context.navigator.showScreen(isLayingOut ? ScreenKind::Placement : ScreenKind::Battle);
    }
} // namespace cpp_warships::game_tui
