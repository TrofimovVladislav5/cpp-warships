#pragma once

#include <application/head/input/Keystroke.h>
#include <application/head/screens/ScreenKind.h>
#include <application/head/views/GameView.h>

namespace cpp_warships::head {
    /** @brief One screen of the interface: something to draw, and something to do with input.
     *  A screen names no drawing library, so any host can show it. */
    class Screen {
    public:
        virtual ~Screen() = default;

        [[nodiscard]] virtual ScreenKind kind() const = 0;

        /** @brief What this screen draws, asked afresh so the latest state is what is shown. */
        [[nodiscard]] virtual GameView& view() = 0;

        /** @brief Reads @p stroke through the view and acts on whatever it turned out to be.
         *  @return whether anything on this screen claimed it. */
        virtual bool handleEvent(const Keystroke& stroke) = 0;
    };
} // namespace cpp_warships::head
