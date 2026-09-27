#include <run_game.h>

#include <build_shell.h>

#include <game_flow/RandomEngine.h>
#include <utilities/Initials.h>

auto main(const int argumentCount, const char* const* arguments) -> int {
    Initials::consoleOutInitials();

    cpp_warships::game_flow::RandomEngine randomEngine =
            cpp_warships::game_flow::makeRandomlySeededEngine();

    const cpp_warships::application::ShellKind shellKind =
            cpp_warships::application::shellKindFromArguments(argumentCount, arguments);

    cpp_warships::application::runGame(randomEngine, shellKind);

    return 0;
}
