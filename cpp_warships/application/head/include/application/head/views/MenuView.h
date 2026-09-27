#pragma once

#include <ftxui/dom/elements.hpp>

#include <application/head/Queries.h>
#include <application/head/Theme.h>
#include <application/head/state/MenuState.h>
#include <application/head/PresentationContext.h>
#include <application/head/views/ftxui_bridge/FtxuiView.h>

namespace cpp_warships::head {
    /** @brief Draws the menu: the title, the board sizes on offer and the themes.
     *  Reads what it was given and returns elements; it changes nothing. */
    class MenuView final : public FtxuiRenderer {
    public:
        explicit MenuView(const PresentationContext& context) noexcept;


    protected:
        [[nodiscard]] ftxui::Element renderElement() override;

    private:
        const PresentationContext& context_;
    };
} // namespace cpp_warships::head
