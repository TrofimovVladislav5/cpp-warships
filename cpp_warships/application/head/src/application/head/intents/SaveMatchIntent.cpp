#include <application/head/intents/SaveMatchIntent.h>

#include <application/head/intents/IntentContext.h>
#include <application/head/session/Application.h>

namespace cpp_warships::head {
    void SaveMatchIntent::applyTo(const IntentContext& context) const {
        context.session.saveMatch();
    }
} // namespace cpp_warships::head
