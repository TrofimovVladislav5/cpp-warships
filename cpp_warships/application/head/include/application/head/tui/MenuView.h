#pragma once

#include <application/head/common/Queries.h>
#include <application/head/common/Theme.h>
#include <application/head/common/state/MenuState.h>
#include <application/head/tui/FtxuiView.h>

#include <ftxui/dom/elements.hpp>

namespace cpp_warships::head::common {
    class PresentationContext;
}

namespace cpp_warships::head::tui {
    /** @brief Draws the menu: the title, the board sizes on offer and the themes. */
    class MenuView final : public FtxuiRenderer {
    public:
        explicit MenuView(const common::PresentationContext& context) noexcept;

    protected:
        [[nodiscard]] ftxui::Element renderElement() override;

    private:
        const common::PresentationContext& context_;
    };
}  // namespace cpp_warships::head::tui
