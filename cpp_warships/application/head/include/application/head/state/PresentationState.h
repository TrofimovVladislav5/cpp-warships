#pragma once

#include <application/head/state/BattleState.h>
#include <application/head/state/MenuState.h>
#include <application/head/state/PlacementState.h>

namespace cpp_warships::head {
    /** @brief Everything the interface remembers that the game itself does not: where the
     *  player is aiming, what they have picked but not yet asked for, how far they have
     *  scrolled. None of it is a game concept, and none of it ever reaches the model. */
    struct PresentationState {
        MenuState menu;
        PlacementState placement;
        BattleState battle;

        /** @brief Whether the player has stepped out to the menu. A match can be in play
         *  while they are looking at the menu, which is why this cannot be read off the
         *  match itself. Everything else about which screen shows can be. */
        bool isAtMenu = true;
    };
} // namespace cpp_warships::head
