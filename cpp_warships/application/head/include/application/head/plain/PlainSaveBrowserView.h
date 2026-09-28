#pragma once

#include <application/head/common/render/Renderer.h>

namespace cpp_warships::head::common {
    class PresentationContext;
}

namespace cpp_warships::head::plain {
    /** @brief The saved games as a plain numbered list, newest first. */
    class PlainSaveBrowserView final : public common::render::Renderer {
    public:
        explicit PlainSaveBrowserView(const common::PresentationContext& context) noexcept;

        [[nodiscard]] common::render::Frame render(
            int availableWidth,
            int availableHeight
        ) override;

    private:
        const common::PresentationContext& context_;
    };
}  // namespace cpp_warships::head::plain
