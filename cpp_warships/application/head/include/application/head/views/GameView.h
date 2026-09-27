#pragma once

#include <application/head/input/InputEvent.h>
#include <application/head/views/Frame.h>

namespace cpp_warships::head {
    /** @brief The one seam between the host and the game.
     *  A view draws what it was given and reads raw input back in the game's own terms.
     *  Nothing outside a view knows where anything landed, and nothing here names a
     *  drawing library, so a view is free to use one or not. */
    class GameView {
    public:
        virtual ~GameView() = default;

        /** @brief Draws the view as things now stand, within the room it has been given.
         *  A width or height of zero means there is no limit; the frame may come back
         *  smaller than offered, and a host that asked for a size will get that size. */
        [[nodiscard]] virtual Frame render(int availableWidth, int availableHeight) = 0;

        /** @brief Reads @p stroke in the game's terms, resolving where it happened. */
        [[nodiscard]] virtual InputEvent interpret(const Keystroke& stroke) const = 0;
    };
} // namespace cpp_warships::head
