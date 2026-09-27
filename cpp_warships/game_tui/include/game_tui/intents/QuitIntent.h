#pragma once

#include <game_tui/intents/Intent.h>

namespace cpp_warships::game_tui {
    /** @brief Asks the application to shut down. */
    class QuitIntent final : public Intent {
    public:
        void applyTo(const IntentContext& context) const override;
    };
} // namespace cpp_warships::game_tui
