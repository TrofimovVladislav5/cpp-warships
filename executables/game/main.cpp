#include <application/flow/RandomEngine.h>
#include <build_shell.h>
#include <run_game.h>
#include <platform/OSBrancher.h>

auto main(const int argumentCount, const char* const* arguments) -> int {
    cpp_warships::platform::prepareConsoleForUnicode();

    cpp_warships::flow::RandomEngine randomEngine = cpp_warships::flow::makeRandomlySeededEngine();

    const cpp_warships::application::ShellKind shellKind =
        cpp_warships::application::shellKindFromArguments(argumentCount, arguments);

    cpp_warships::application::runGame(randomEngine, shellKind);

    return 0;
}
