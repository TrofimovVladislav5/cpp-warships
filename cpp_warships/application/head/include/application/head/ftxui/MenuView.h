#pragma once

#include <application/head/common/PresentationContext.h>
#include <application/head/common/Queries.h>
#include <application/head/common/Theme.h>
#include <application/head/common/state/MenuState.h>
#include <application/head/ftxui/FtxuiView.h>

#include <ftxui/dom/elements.hpp>

namespace cpp_warships::head {
    /** @brief Draws the menu: the title, the board sizes on offer and the
     * themes. Reads what it was given and returns elements; it changes nothing.
     */
    class MenuView final : public FtxuiRenderer {
       public:
        explicit MenuView(const PresentationContext& context) noexcept;

       protected:
        [[nodiscard]] ftxui::Element renderElement() override;

       private:
        const PresentationContext& context_;
    };
}  // namespace cpp_warships::head
