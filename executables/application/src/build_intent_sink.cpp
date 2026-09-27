#include <build_intent_sink.h>

#include <utility>

namespace cpp_warships::application {
    game_tui::IntentSink buildIntentSink(const game_tui::IntentContext& context) {
        return [&context](game_tui::IntentPointer intent) {
            if (intent) {
                intent->applyTo(context);
            }
        };
    }
} // namespace cpp_warships::application
