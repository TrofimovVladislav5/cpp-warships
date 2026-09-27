#include <application/head/intents/FireAtIntent.h>

#include <application/head/intents/IntentContext.h>
#include <application/head/session/Application.h>

namespace cpp_warships::head {
    FireAtIntent::FireAtIntent(core::Coordinate coordinate)
        : coordinate_(coordinate) {}

    void FireAtIntent::applyTo(const IntentContext& context) const {
        context.session.fireAt(coordinate_);
    }
} // namespace cpp_warships::head
