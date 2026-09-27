#include <application/head/intents/ShowScreenIntent.h>

#include <application/head/intents/IntentContext.h>
#include <application/head/screens/ScreenNavigator.h>
#include <application/head/session/Application.h>

namespace cpp_warships::head {
    ShowScreenIntent::ShowScreenIntent(ScreenKind screen)
        : screen_(screen) {}

    void ShowScreenIntent::applyTo(const IntentContext& context) const {
        context.navigator.showScreen(screen_);
    }
} // namespace cpp_warships::head
