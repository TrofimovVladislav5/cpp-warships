#pragma once

#include <application/head/intents/Intent.h>

namespace cpp_warships::head {
    /** @brief Asks the application to pick the saved match back up, and shows where it left off. */
    class LoadMatchIntent final : public Intent {
    public:
        void applyTo(const IntentContext& context) const override;
    };
} // namespace cpp_warships::head
