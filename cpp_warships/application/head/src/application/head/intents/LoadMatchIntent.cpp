#include <application/head/intents/LoadMatchIntent.h>

#include <application/head/intents/IntentContext.h>
#include <application/head/screens/ScreenKind.h>
#include <application/head/screens/ScreenNavigator.h>
#include <application/model/ApplicationContext.h>

namespace cpp_warships::head {
    void LoadMatchIntent::applyTo(const IntentContext& context) const {
        if (!context.application.game().saves().loadMatch()) {
            return;
        }

        // A save remembers where play had got to, so it reopens on the screen it was left on.
        const bool isLayingOut =
                context.application.game().match().phase() == flow::MatchPhase::Placement;
        context.navigator.showScreen(isLayingOut ? ScreenKind::Placement : ScreenKind::Battle);
    }
} // namespace cpp_warships::head
