#pragma once

#include <application/head/common/state/BattleState.h>
#include <application/head/common/state/MenuState.h>
#include <application/head/common/state/PlacementState.h>

namespace cpp_warships::head::common::state {
    /** @brief Everything the interface remembers that the game itself does not: where the player
     * is aiming, what they have picked but not yet asked for, how far they have scrolled. */
    struct PresentationState {
        MenuState menu;
        PlacementState placement;
        BattleState battle;

        /** @brief Whether the player has stepped out to the menu. */
        bool isAtMenu = true;
    };
}  // namespace cpp_warships::head::common::state
