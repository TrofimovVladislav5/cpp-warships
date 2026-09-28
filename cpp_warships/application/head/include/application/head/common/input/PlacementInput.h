#pragma once

#include <application/head/common/input/BoundScreenInput.h>

namespace cpp_warships::head::common {
    class PresentationContext;
}

namespace cpp_warships::head::common::input {
    /** @brief Laying a fleet out, read in the game's terms. Where the player is aiming and
     * which way the ship lies are settled here: the game hears only the finished ask. */
    class PlacementInput final : public BoundScreenInput {
    public:
        explicit PlacementInput(PresentationContext& context);
    };
}  // namespace cpp_warships::head::common::input
