#pragma once

#include <game_tui/intents/Intent.h>

namespace cpp_warships::game_tui {
    /** @brief Asks the application to pick the saved match back up, and shows where it left off. */
    class LoadMatchIntent final : public Intent {
    public:
        void applyTo(const IntentContext& context) const override;
    };
} // namespace cpp_warships::game_tui
