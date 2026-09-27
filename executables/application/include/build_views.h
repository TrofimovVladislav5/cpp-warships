#pragma once

#include <build_shell.h>

#include <game_tui/views/ViewFactory.h>

namespace cpp_warships::application {
    /** @brief The presentation a host of @p kind is shown through.
     *  This is the only place that says which set of views the game is drawn with. */
    [[nodiscard]] game_tui::ViewFactory buildViewFactory(ShellKind kind);
} // namespace cpp_warships::application
