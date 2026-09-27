#include <game_tui/intents/FireAtIntent.h>

#include <game_tui/intents/IntentContext.h>
#include <game_tui/session/Application.h>

namespace cpp_warships::game_tui {
    FireAtIntent::FireAtIntent(game_core::Coordinate coordinate)
        : coordinate_(coordinate) {}

    void FireAtIntent::applyTo(const IntentContext& context) const {
        context.session.fireAt(coordinate_);
    }
} // namespace cpp_warships::game_tui
