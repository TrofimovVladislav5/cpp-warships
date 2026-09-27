#pragma once

#include <optional>

#include <game_core/Coordinate.h>
#include <game_tui/input/Keystroke.h>

namespace cpp_warships::game_tui {
    /** @brief The part of a screen a pointer is over, named in the game's terms.
     *  Only a view knows where things were drawn, so only a view decides this. */
    enum class InputRegion {
        Elsewhere,
        OwnWaters,
        EnemyWaters,
        Log,
    };

    /** @brief What the player did, once a view has read it in the game's own terms.
     *  Handlers work from this, so none of them needs to know how anything was drawn. */
    struct InputEvent {
        Keystroke stroke;
        InputRegion region = InputRegion::Elsewhere;

        /** @brief The board cell under the pointer, when the pointer is over a board. */
        std::optional<game_core::Coordinate> cell;
    };
} // namespace cpp_warships::game_tui
