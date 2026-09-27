#pragma once

#include <application/core/Coordinate.h>
#include <application/core/Direction.h>
#include <application/head/intents/Intent.h>

namespace cpp_warships::head {
    /** @brief Asks the application to lay a ship of the given length on the player's board. */
    class PlaceShipIntent final : public Intent {
    public:
        PlaceShipIntent(core::Coordinate origin, core::Direction direction, int length);

        void applyTo(const IntentContext& context) const override;

    private:
        core::Coordinate origin_;
        core::Direction direction_;
        int length_;
    };
} // namespace cpp_warships::head
