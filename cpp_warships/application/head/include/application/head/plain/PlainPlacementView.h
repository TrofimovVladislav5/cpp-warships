#pragma once

#include <application/head/common/Queries.h>
#include <application/head/common/Theme.h>
#include <application/head/common/render/Renderer.h>
#include <application/head/common/state/PlacementState.h>

namespace cpp_warships::head::common {
    class PresentationContext;
}

namespace cpp_warships::head::plain {
    /** @brief The fleet being laid out, printed as the console game used to
     * print it: the board ruled out in ASCII, the roster under it as a list. */
    class PlainPlacementView final : public common::render::Renderer {
    public:
        explicit PlainPlacementView(const common::PresentationContext& context) noexcept;

        [[nodiscard]] common::render::Frame render(
            int availableWidth,
            int availableHeight
        ) override;

    private:
        const common::PresentationContext& context_;
    };
}  // namespace cpp_warships::head::plain
