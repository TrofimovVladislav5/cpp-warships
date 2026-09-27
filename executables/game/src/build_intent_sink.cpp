#include <build_intent_sink.h>

#include <utility>

namespace cpp_warships::application {
    head::IntentSink buildIntentSink(const head::IntentContext& context) {
        return [&context](head::IntentPointer intent) {
            if (intent) {
                intent->applyTo(context);
            }
        };
    }
} // namespace cpp_warships::application
