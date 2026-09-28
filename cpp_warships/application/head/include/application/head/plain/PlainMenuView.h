#pragma once

#include <application/head/common/Queries.h>
#include <application/head/common/Theme.h>
#include <application/head/common/render/Renderer.h>
#include <application/head/common/state/MenuState.h>

namespace cpp_warships::head::common {
    class PresentationContext;
}

namespace cpp_warships::head::plain {
    /** @brief The menu as the console game used to print it: a few lines, no
     * colour. Reads what it was given and returns text; it changes nothing. */
    class PlainMenuView final : public common::render::Renderer {
    public:
        explicit PlainMenuView(const common::PresentationContext& context) noexcept;

        [[nodiscard]] common::render::Frame render(
            int availableWidth,
            int availableHeight
        ) override;

    private:
        const common::PresentationContext& context_;
    };
}  // namespace cpp_warships::head::plain
