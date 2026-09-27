#pragma once

#include <build_shell.h>

#include <application/head/PresentationContext.h>
#include <application/head/views/RendererSet.h>

namespace cpp_warships::application {
    /** @brief A renderer for every screen, drawn the way @p kind calls for, all reading
     *  @p context. This is the one place that says which presentation is in use, and
     *  swapping a whole look is swapping nothing but this. */
    [[nodiscard]] head::RendererSet buildRenderers(
            ShellKind kind,
            head::PresentationContext& context
    );
} // namespace cpp_warships::application
