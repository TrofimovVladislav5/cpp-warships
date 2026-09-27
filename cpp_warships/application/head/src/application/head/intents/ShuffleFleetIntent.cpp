#include <application/head/intents/ShuffleFleetIntent.h>

#include <application/head/intents/IntentContext.h>
#include <application/model/ApplicationContext.h>

namespace cpp_warships::head {
    void ShuffleFleetIntent::applyTo(const IntentContext& context) const {
        context.application.game().play().shuffleFleet();
    }
} // namespace cpp_warships::head
