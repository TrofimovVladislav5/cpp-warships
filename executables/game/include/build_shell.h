#pragma once

#include <application/head/common/Queries.h>
#include <application/head/common/host/Shell.h>

#include <memory>

namespace cpp_warships::application {
    /** @brief The hosts the game can be shown in, chosen once when the program starts. */
    enum class ShellKind {
        InteractiveTerminal,
        PlainTerminal,
    };

    /** @brief The host the player asked for on the command line. */
    [[nodiscard]] ShellKind shellKindFromArguments(int argumentCount, const char* const* arguments);

    /** @brief A host of @p kind, dressed in the colours @p theme answers with. */
    [[nodiscard]] std::unique_ptr<head::common::host::Shell> buildShell(
        ShellKind kind,
        head::common::ThemeQuery theme
    );
}  // namespace cpp_warships::application
