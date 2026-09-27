#include <application/head/intents/ResumeMatchIntent.h>

#include <application/head/intents/IntentContext.h>
#include <application/head/screens/ScreenKind.h>
#include <application/head/screens/ScreenNavigator.h>
#include <application/head/session/Application.h>

namespace cpp_warships::head {
    void ResumeMatchIntent::applyTo(const IntentContext& context) const {
        if (!context.session.hasMatch()) {
            return;
        }

        const bool isLayingOut =
                context.session.match().phase() == flow::MatchPhase::Placement;
        context.navigator.showScreen(isLayingOut ? ScreenKind::Placement : ScreenKind::Battle);
    }
} // namespace cpp_warships::head
