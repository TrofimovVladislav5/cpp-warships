#pragma once

#include <application/core/Coordinate.h>
#include <application/head/intents/Intent.h>

namespace cpp_warships::head {
    /** @brief Asks the application to take back the ship covering a cell. */
    class RemoveShipIntent final : public Intent {
    public:
        explicit RemoveShipIntent(core::Coordinate coordinate);

        void applyTo(const IntentContext& context) const override;

    private:
        core::Coordinate coordinate_;
    };
} // namespace cpp_warships::head
