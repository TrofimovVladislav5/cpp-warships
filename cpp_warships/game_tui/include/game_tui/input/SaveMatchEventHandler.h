#pragma once

#include <game_tui/input/EventHandler.h>
#include <game_tui/intents/Intent.h>

namespace cpp_warships::game_tui {
    /** @brief Writes the match in play over the save, from wherever the player is in it. */
    class SaveMatchEventHandler final : public EventHandler {
    public:
        explicit SaveMatchEventHandler(IntentSink intentSink);

        [[nodiscard]] bool isHandled(const InputEvent& input) const override;
        void handleEvent(const InputEvent& input) override;

    private:
        IntentSink intentSink_;
    };
} // namespace cpp_warships::game_tui
