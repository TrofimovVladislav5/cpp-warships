#pragma once

#include <application/head/common/render/Renderer.h>

namespace cpp_warships::head::common {
    class PresentationContext;
}

namespace cpp_warships::head::plain {
    /** @brief The prompt asking what to call the match being put away. */
    class PlainSaveNamingView final : public common::render::Renderer {
    public:
        explicit PlainSaveNamingView(const common::PresentationContext& context) noexcept;

        [[nodiscard]] common::render::Frame render(
            int availableWidth,
            int availableHeight
        ) override;

    private:
        const common::PresentationContext& context_;
    };
}  // namespace cpp_warships::head::plain
