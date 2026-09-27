#include <application/head/intents/StartMatchIntent.h>

#include <application/head/intents/IntentContext.h>
#include <application/head/screens/ScreenKind.h>
#include <application/head/screens/ScreenNavigator.h>
#include <application/model/ApplicationContext.h>

namespace cpp_warships::head {
    StartMatchIntent::StartMatchIntent(int boardSize)
        : boardSize_(boardSize) {}

    void StartMatchIntent::applyTo(const IntentContext& context) const {
        context.application.game().play().startNewMatch(boardSize_);
        context.navigator.showScreen(ScreenKind::Placement);
    }
} // namespace cpp_warships::head
