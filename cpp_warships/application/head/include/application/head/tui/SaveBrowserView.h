#pragma once

#include <application/head/tui/FtxuiView.h>

namespace cpp_warships::head::common {
    class PresentationContext;
}

namespace cpp_warships::head::tui {
    /** @brief The saved games as a highlighted list, newest first. */
    class SaveBrowserView final : public FtxuiRenderer {
    public:
        explicit SaveBrowserView(const common::PresentationContext& context) noexcept;

    protected:
        [[nodiscard]] ftxui::Element renderElement() override;

    private:
        const common::PresentationContext& context_;
    };
}  // namespace cpp_warships::head::tui
