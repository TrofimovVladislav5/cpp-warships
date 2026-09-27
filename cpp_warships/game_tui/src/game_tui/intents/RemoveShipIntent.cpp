#include <game_tui/intents/RemoveShipIntent.h>

#include <game_tui/intents/IntentContext.h>
#include <game_tui/session/Application.h>

namespace cpp_warships::game_tui {
    RemoveShipIntent::RemoveShipIntent(game_core::Coordinate coordinate)
        : coordinate_(coordinate) {}

    void RemoveShipIntent::applyTo(const IntentContext& context) const {
        context.session.removeShipAt(coordinate_);
    }
} // namespace cpp_warships::game_tui
