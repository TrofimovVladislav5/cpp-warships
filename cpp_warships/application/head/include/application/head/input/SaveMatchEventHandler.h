#pragma once

#include <application/head/input/EventHandler.h>
#include <application/head/intents/Intent.h>

namespace cpp_warships::head {
    /** @brief Writes the match in play over the save, from wherever the player is in it. */
    class SaveMatchEventHandler final : public EventHandler {
    public:
        explicit SaveMatchEventHandler(IntentSink intentSink);

        [[nodiscard]] bool isHandled(const InputEvent& input) const override;
        void handleEvent(const InputEvent& input) override;

    private:
        IntentSink intentSink_;
    };
} // namespace cpp_warships::head
