#pragma once

#include <application/head/common/Queries.h>
#include <application/head/common/host/Shell.h>

#include <memory>

namespace cpp_warships::application {
    /** @brief The hosts the game can be shown in, chosen once when the program
     * starts. Adding one here is the whole cost of another way to present the
     * same screens. */
    enum class ShellKind {
        InteractiveTerminal,
        PlainTerminal,
    };

    /** @brief The host the player asked for on the command line.
     *  Anything but "--plain" leaves the interactive terminal, which is the
     * usual way in. */
    [[nodiscard]] ShellKind shellKindFromArguments(int argumentCount, const char* const* arguments);

    /** @brief A host of @p kind, dressed in the colours @p theme answers with.
     */
    [[nodiscard]] std::unique_ptr<head::Shell> buildShell(ShellKind kind, head::ThemeQuery theme);
}  // namespace cpp_warships::application
