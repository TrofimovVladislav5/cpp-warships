#include <application/head/intents/PlaceShipIntent.h>

#include <application/head/intents/IntentContext.h>
#include <application/model/ApplicationContext.h>

namespace cpp_warships::head {
    PlaceShipIntent::PlaceShipIntent(
            core::Coordinate origin,
            core::Direction direction,
            int length
    )
        : origin_(origin)
        , direction_(direction)
        , length_(length) {}

    void PlaceShipIntent::applyTo(const IntentContext& context) const {
        context.application.game().play().placeShip(origin_, direction_, length_);
    }
} // namespace cpp_warships::head
