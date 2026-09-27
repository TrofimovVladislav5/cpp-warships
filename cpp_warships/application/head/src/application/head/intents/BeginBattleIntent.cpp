#include <application/head/intents/BeginBattleIntent.h>

#include <application/head/intents/IntentContext.h>
#include <application/head/screens/ScreenKind.h>
#include <application/head/screens/ScreenNavigator.h>
#include <application/model/ApplicationContext.h>

namespace cpp_warships::head {
    void BeginBattleIntent::applyTo(const IntentContext& context) const {
        if (context.application.game().play().beginBattle()) {
            context.navigator.showScreen(ScreenKind::Battle);
        }
    }
} // namespace cpp_warships::head
