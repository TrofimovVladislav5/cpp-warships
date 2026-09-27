#pragma once

#include <memory>

#include <game_tui/Queries.h>
#include <game_tui/host/Shell.h>

namespace cpp_warships::application {
    /** @brief The hosts the game can be shown in, chosen once when the program starts.
     *  Adding one here is the whole cost of another way to present the same screens. */
    enum class ShellKind {
        InteractiveTerminal,
        PlainTerminal,
    };

    /** @brief The host the player asked for on the command line.
     *  Anything but "--plain" leaves the interactive terminal, which is the usual way in. */
    [[nodiscard]] ShellKind shellKindFromArguments(int argumentCount, const char* const* arguments);

    /** @brief A host of @p kind, dressed in the colours @p theme answers with. */
    [[nodiscard]] std::unique_ptr<game_tui::Shell> buildShell(
            ShellKind kind,
            game_tui::ThemeQuery theme
    );
} // namespace cpp_warships::application
