#include <game_tui/intents/QuitIntent.h>

#include <game_tui/host/Shell.h>
#include <game_tui/intents/IntentContext.h>
#include <game_tui/session/Application.h>

namespace cpp_warships::game_tui {
    void QuitIntent::applyTo(const IntentContext& context) const {
        context.shell.requestQuit();
    }
} // namespace cpp_warships::game_tui
