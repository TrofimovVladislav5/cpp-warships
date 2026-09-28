#pragma once

#include <application/head/common/render/Frame.h>

#include <memory>

namespace cpp_warships::head::common::render {
    /** @brief Draws one screen from the context it was given. */
    class Renderer {
    public:
        virtual ~Renderer() = default;

        /** @brief What this screen looks like, given the room it has to fill. */
        [[nodiscard]] virtual Frame render(int availableWidth, int availableHeight) = 0;
    };

    using RendererPointer = std::unique_ptr<Renderer>;
}  // namespace cpp_warships::head::common::render
