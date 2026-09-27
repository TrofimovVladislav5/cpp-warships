#pragma once

#include <game_tui/intents/Intent.h>
#include <game_tui/screens/ScreenKind.h>

namespace cpp_warships::game_tui {
    /** @brief Asks the application to show another screen. */
    class ShowScreenIntent final : public Intent {
    public:
        explicit ShowScreenIntent(ScreenKind screen);

        void applyTo(const IntentContext& context) const override;

    private:
        ScreenKind screen_;
    };
} // namespace cpp_warships::game_tui
