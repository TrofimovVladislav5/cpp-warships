#pragma once

#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>

#include <application/head/views/GameView.h>

namespace cpp_warships::head {
    /** @brief What @p event is, said without naming FTXUI, so the game never has to ask. */
    [[nodiscard]] Keystroke keystrokeOf(ftxui::Event event);

    /** @brief @p frame as something FTXUI can lay out and draw, cell for cell. */
    [[nodiscard]] ftxui::Element elementOfFrame(const Frame& frame);

    /** @brief A view that draws with FTXUI, handing back a finished frame like any other.
     *  Subclasses build elements and never think about frames; this turns one into the other. */
    class FtxuiView : public GameView {
    public:
        [[nodiscard]] Frame render(int availableWidth, int availableHeight) final;

    protected:
        /** @brief Draws the view as an element tree, laid out in the room it was offered. */
        [[nodiscard]] virtual ftxui::Element renderElement() = 0;
    };
} // namespace cpp_warships::head
