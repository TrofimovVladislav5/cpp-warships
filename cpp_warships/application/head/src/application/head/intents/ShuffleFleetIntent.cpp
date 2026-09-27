#include <application/head/intents/ShuffleFleetIntent.h>

#include <application/head/intents/IntentContext.h>
#include <application/head/session/Application.h>

namespace cpp_warships::head {
    void ShuffleFleetIntent::applyTo(const IntentContext& context) const {
        context.session.shuffleFleet();
    }
} // namespace cpp_warships::head
