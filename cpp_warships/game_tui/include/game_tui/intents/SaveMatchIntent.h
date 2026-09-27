#pragma once

#include <game_tui/intents/Intent.h>

namespace cpp_warships::game_tui {
    /** @brief Asks the application to write the match in play over the save. */
    class SaveMatchIntent final : public Intent {
    public:
        void applyTo(const IntentContext& context) const override;
    };
} // namespace cpp_warships::game_tui
