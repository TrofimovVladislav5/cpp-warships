#include <application/head/intents/RemoveShipIntent.h>

#include <application/head/intents/IntentContext.h>
#include <application/head/session/Application.h>

namespace cpp_warships::head {
    RemoveShipIntent::RemoveShipIntent(core::Coordinate coordinate)
        : coordinate_(coordinate) {}

    void RemoveShipIntent::applyTo(const IntentContext& context) const {
        context.session.removeShipAt(coordinate_);
    }
} // namespace cpp_warships::head
