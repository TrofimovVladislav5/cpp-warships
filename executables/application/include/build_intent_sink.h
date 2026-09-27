#pragma once

#include <game_tui/intents/Intent.h>
#include <game_tui/intents/IntentContext.h>

namespace cpp_warships::application {
    /** @brief Where raised intents go: straight at @p context, which must outlive the sink.
     *  This is the whole of the one-way road from what a screen asks for to what happens. */
    [[nodiscard]] game_tui::IntentSink buildIntentSink(const game_tui::IntentContext& context);
} // namespace cpp_warships::application
