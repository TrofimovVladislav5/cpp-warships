#pragma once

#include <application/head/common/ScreenKind.h>
#include <application/head/common/render/Renderer.h>

#include <map>

namespace cpp_warships::head::common::render {
    /** @brief One whole way of presenting the game: a renderer for every screen there is. */
    class RendererSet {
    public:
        void drawScreenWith(ScreenKind screen, RendererPointer renderer);

        /** @brief What @p screen looks like now, or an empty frame when nothing
         * draws it. */
        [[nodiscard]] Frame render(ScreenKind screen, int availableWidth, int availableHeight);

    private:
        std::map<ScreenKind, RendererPointer> renderers_;
    };
}  // namespace cpp_warships::head::common::render
