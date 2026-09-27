#include <application/head/intents/SaveMatchIntent.h>

#include <application/head/intents/IntentContext.h>
#include <application/model/ApplicationContext.h>

namespace cpp_warships::head {
    void SaveMatchIntent::applyTo(const IntentContext& context) const {
        context.application.game().saves().saveMatch();
    }
} // namespace cpp_warships::head
