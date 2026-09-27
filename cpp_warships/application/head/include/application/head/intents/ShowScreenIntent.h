#pragma once

#include <application/head/intents/Intent.h>
#include <application/head/screens/ScreenKind.h>

namespace cpp_warships::head {
    /** @brief Asks the application to show another screen. */
    class ShowScreenIntent final : public Intent {
    public:
        explicit ShowScreenIntent(ScreenKind screen);

        void applyTo(const IntentContext& context) const override;

    private:
        ScreenKind screen_;
    };
} // namespace cpp_warships::head
