#pragma once

#include <application/head/intents/Intent.h>

namespace cpp_warships::head {
    /** @brief Asks the application to reopen the match in play, wherever it left off. */
    class ResumeMatchIntent final : public Intent {
    public:
        void applyTo(const IntentContext& context) const override;
    };
} // namespace cpp_warships::head
