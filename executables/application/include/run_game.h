#pragma once

#include <build_shell.h>

#include <game_flow/RandomEngine.h>

namespace cpp_warships::application {
    /** @brief Puts the parts together and shows the game in a host of @p shellKind, until
     *  the player quits. Every other entry point, terminal or otherwise, comes through here. */
    void runGame(game_flow::RandomEngine& randomEngine, ShellKind shellKind);
} // namespace cpp_warships::application
