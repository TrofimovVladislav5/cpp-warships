#pragma once

#include <application/head/tui/FtxuiView.h>

namespace cpp_warships::head::common {
    class PresentationContext;
}

namespace cpp_warships::head::tui {
    /** @brief The prompt asking what to call the match being put away. */
    class SaveNamingView final : public FtxuiRenderer {
    public:
        explicit SaveNamingView(const common::PresentationContext& context) noexcept;

    protected:
        [[nodiscard]] ftxui::Element renderElement() override;

    private:
        const common::PresentationContext& context_;
    };
}  // namespace cpp_warships::head::tui
