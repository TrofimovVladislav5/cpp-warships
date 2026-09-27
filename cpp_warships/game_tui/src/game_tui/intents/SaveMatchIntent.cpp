#include <game_tui/intents/SaveMatchIntent.h>

#include <game_tui/intents/IntentContext.h>
#include <game_tui/session/Application.h>

namespace cpp_warships::game_tui {
    void SaveMatchIntent::applyTo(const IntentContext& context) const {
        context.session.saveMatch();
    }
} // namespace cpp_warships::game_tui
