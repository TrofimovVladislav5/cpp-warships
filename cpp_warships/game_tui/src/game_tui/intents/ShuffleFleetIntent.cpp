#include <game_tui/intents/ShuffleFleetIntent.h>

#include <game_tui/intents/IntentContext.h>
#include <game_tui/session/Application.h>

namespace cpp_warships::game_tui {
    void ShuffleFleetIntent::applyTo(const IntentContext& context) const {
        context.session.shuffleFleet();
    }
} // namespace cpp_warships::game_tui
