#pragma once

#include <application/head/common/PresentationContext.h>
#include <application/head/common/render/RendererSet.h>
#include <build_shell.h>

namespace cpp_warships::application {
    /** @brief A renderer for every screen, drawn the way @p kind calls for, all
     * reading @p context. */
    [[nodiscard]] head::RendererSet
    buildRenderers(ShellKind kind, head::PresentationContext& context);
}  // namespace cpp_warships::application
