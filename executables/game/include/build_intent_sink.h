#pragma once

#include <application/head/intents/Intent.h>
#include <application/head/intents/IntentContext.h>

namespace cpp_warships::application {
    /** @brief Where raised intents go: straight at @p context, which must outlive the sink.
     *  This is the whole of the one-way road from what a screen asks for to what happens. */
    [[nodiscard]] head::IntentSink buildIntentSink(const head::IntentContext& context);
} // namespace cpp_warships::application
