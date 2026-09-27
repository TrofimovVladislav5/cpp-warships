#pragma once

#include <application/head/intents/Intent.h>

namespace cpp_warships::head {
    /** @brief Asks the application to leave placement and open fire. */
    class BeginBattleIntent final : public Intent {
    public:
        void applyTo(const IntentContext& context) const override;
    };
} // namespace cpp_warships::head
