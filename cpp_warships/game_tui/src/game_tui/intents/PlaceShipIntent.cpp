#include <game_tui/intents/PlaceShipIntent.h>

#include <game_tui/intents/IntentContext.h>
#include <game_tui/session/Application.h>

namespace cpp_warships::game_tui {
    PlaceShipIntent::PlaceShipIntent(
            game_core::Coordinate origin,
            game_core::Direction direction,
            int length
    )
        : origin_(origin)
        , direction_(direction)
        , length_(length) {}

    void PlaceShipIntent::applyTo(const IntentContext& context) const {
        context.session.placeShip(origin_, direction_, length_);
    }
} // namespace cpp_warships::game_tui
