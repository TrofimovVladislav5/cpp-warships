#pragma once

#include <application/core/Coordinate.h>
#include <application/head/intents/Intent.h>

namespace cpp_warships::head {
    /** @brief Asks the application to fire at a cell of the enemy's waters. */
    class FireAtIntent final : public Intent {
    public:
        explicit FireAtIntent(core::Coordinate coordinate);

        void applyTo(const IntentContext& context) const override;

    private:
        core::Coordinate coordinate_;
    };
} // namespace cpp_warships::head
