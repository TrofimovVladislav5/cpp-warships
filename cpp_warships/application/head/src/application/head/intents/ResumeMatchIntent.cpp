#include <application/head/intents/ResumeMatchIntent.h>

#include <application/head/intents/IntentContext.h>
#include <application/head/screens/ScreenKind.h>
#include <application/head/screens/ScreenNavigator.h>
#include <application/model/ApplicationContext.h>

namespace cpp_warships::head {
    void ResumeMatchIntent::applyTo(const IntentContext& context) const {
        if (!context.application.game().hasMatch()) {
            return;
        }

        const bool isLayingOut =
                context.application.game().match().phase() == flow::MatchPhase::Placement;
        context.navigator.showScreen(isLayingOut ? ScreenKind::Placement : ScreenKind::Battle);
    }
} // namespace cpp_warships::head
