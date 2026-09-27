#pragma once

#include <build_shell.h>

#include <application/head/views/ViewFactory.h>

namespace cpp_warships::application {
    /** @brief The presentation a host of @p kind is shown through.
     *  This is the only place that says which set of views the game is drawn with. */
    [[nodiscard]] head::ViewFactory buildViewFactory(ShellKind kind);
} // namespace cpp_warships::application
