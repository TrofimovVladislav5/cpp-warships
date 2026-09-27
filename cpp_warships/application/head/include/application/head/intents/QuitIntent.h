#pragma once

#include <application/head/intents/Intent.h>

namespace cpp_warships::head {
    /** @brief Asks the application to shut down. */
    class QuitIntent final : public Intent {
    public:
        void applyTo(const IntentContext& context) const override;
    };
} // namespace cpp_warships::head
