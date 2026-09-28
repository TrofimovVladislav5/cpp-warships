#pragma once

#include <application/head/common/input/Keystroke.h>
#include <application/head/common/render/Renderer.h>

#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>

namespace cpp_warships::head::tui {
    /** @brief What @p event is, said without naming FTXUI, so the game never
     * has to ask. */
    [[nodiscard]] common::input::Keystroke keystrokeOf(ftxui::Event event);

    /** @brief @p frame as something FTXUI can lay out and draw, cell for cell. */
    [[nodiscard]] ftxui::Element elementOfFrame(const common::render::Frame& frame);

    /** @brief A view that draws with FTXUI, handing back a finished frame like any other. */
    class FtxuiRenderer : public common::render::Renderer {
    public:
        [[nodiscard]] common::render::Frame render(int availableWidth, int availableHeight) final;

    protected:
        /** @brief Draws the view as an element tree, laid out in the room it
         * was offered. */
        [[nodiscard]] virtual ftxui::Element renderElement() = 0;
    };
}  // namespace cpp_warships::head::tui
