#pragma once

#include <application/flow/RandomEngine.h>
#include <build_shell.h>

namespace cpp_warships::application {
    /** @brief Puts the parts together and shows the game in a host of @p shellKind, until the
     * player quits. */
    void runGame(flow::RandomEngine& randomEngine, ShellKind shellKind);
}  // namespace cpp_warships::application
