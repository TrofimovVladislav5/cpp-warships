#pragma once

#include <string>

#include <application/head/intents/Intent.h>

namespace cpp_warships::head {
    /** @brief Asks the application to dress itself in another theme. */
    class ChangeThemeIntent final : public Intent {
    public:
        explicit ChangeThemeIntent(std::string themeName);

        void applyTo(const IntentContext& context) const override;

    private:
        std::string themeName_;
    };
} // namespace cpp_warships::head
