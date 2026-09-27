#pragma once

#include <map>

#include <application/head/screens/ScreenKind.h>
#include <application/head/views/Renderer.h>

namespace cpp_warships::head {
    /** @brief One whole way of presenting the game: a renderer for every screen there is.
     *  Swapping the presentation is swapping this, and nothing else. */
    class RendererSet {
    public:
        void drawScreenWith(ScreenKind screen, RendererPointer renderer);

        /** @brief What @p screen looks like now, or an empty frame when nothing draws it. */
        [[nodiscard]] Frame render(ScreenKind screen, int availableWidth, int availableHeight);

    private:
        std::map<ScreenKind, RendererPointer> renderers_;
    };
} // namespace cpp_warships::head
