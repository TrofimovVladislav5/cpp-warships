#include <application/head/common/render/RendererSet.h>

#include <utility>

namespace cpp_warships::head {
    void RendererSet::drawScreenWith(const ScreenKind screen, RendererPointer renderer) {
        renderers_[screen] = std::move(renderer);
    }

    Frame RendererSet::render(
        const ScreenKind screen,
        const int availableWidth,
        const int availableHeight
    ) {
        const auto drawing = renderers_.find(screen);
        if (drawing == renderers_.end()) {
            return {};
        }

        return drawing->second->render(availableWidth, availableHeight);
    }
}  // namespace cpp_warships::head
