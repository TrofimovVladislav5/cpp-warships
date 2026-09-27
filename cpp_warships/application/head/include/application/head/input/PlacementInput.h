#pragma once

#include <application/head/PresentationContext.h>
#include <application/head/input/ScreenInput.h>

namespace cpp_warships::head {
    /** @brief Laying out a fleet, read in the game's terms. Where the player is aiming and
     *  which way the ship in hand lies are settled here: the game hears only the finished ask. */
    class PlacementInput final : public ScreenInput {
    public:
        /** @brief Reads onto @p context, which must outlive it. */
        explicit PlacementInput(PresentationContext& context) noexcept;

        [[nodiscard]] std::optional<model::GameEvent> interpret(const Keystroke& stroke) override;

    private:
        [[nodiscard]] std::optional<model::GameEvent> interpretPointer(const Keystroke& stroke);

        /** @brief The ask to lay the ship in hand where the cursor rests, if one is in hand. */
        [[nodiscard]] std::optional<model::GameEvent> layShipInHand() const;

        void moveCursor(const Keystroke& stroke);
        void turnShip();
        void pickNextShipLength();

        PresentationContext& context_;
    };
} // namespace cpp_warships::head
