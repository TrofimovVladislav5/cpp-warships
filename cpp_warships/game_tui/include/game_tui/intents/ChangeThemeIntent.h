#pragma once

#include <string>

#include <game_tui/intents/Intent.h>

namespace cpp_warships::game_tui {
    /** @brief Asks the application to dress itself in another theme. */
    class ChangeThemeIntent final : public Intent {
    public:
        explicit ChangeThemeIntent(std::string themeName);

        void applyTo(const IntentContext& context) const override;

    private:
        std::string themeName_;
    };
} // namespace cpp_warships::game_tui
