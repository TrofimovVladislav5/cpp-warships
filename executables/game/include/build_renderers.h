#pragma once

#include <application/head/common/render/RendererSet.h>
#include <build_shell.h>

namespace cpp_warships::head::common {
    class PresentationContext;
}

namespace cpp_warships::application {
    /** @brief A renderer for every screen, drawn the way @p kind calls for, all
     * reading @p context. */
    [[nodiscard]] head::common::render::RendererSet buildRenderers(
        ShellKind kind,
        head::common::PresentationContext& context
    );
}  // namespace cpp_warships::application
