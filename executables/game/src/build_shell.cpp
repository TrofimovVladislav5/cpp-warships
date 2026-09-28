#include <application/head/plain/TerminalShell.h>
#include <application/head/tui/TuiShell.h>
#include <build_shell.h>

#include <algorithm>
#include <iostream>
#include <string_view>
#include <utility>

namespace cpp_warships::application {
    namespace {
        constexpr std::string_view PLAIN_TERMINAL_ARGUMENT = "--plain";
    }  // namespace

    ShellKind shellKindFromArguments(const int argumentCount, const char* const* arguments) {
        const auto isPlainTerminal = [](const char* argument) {
            return argument != nullptr && argument == PLAIN_TERMINAL_ARGUMENT;
        };

        const bool wantsPlainTerminal =
            std::any_of(arguments, arguments + argumentCount, isPlainTerminal);

        return wantsPlainTerminal ? ShellKind::PlainTerminal : ShellKind::InteractiveTerminal;
    }

    std::unique_ptr<head::common::host::Shell> buildShell(
        const ShellKind kind,
        head::common::ThemeQuery theme
    ) {
        if (kind == ShellKind::PlainTerminal) {
            return std::make_unique<head::plain::TerminalShell>(std::cin, std::cout);
        }

        return std::make_unique<head::tui::TuiShell>(std::move(theme));
    }
}  // namespace cpp_warships::application
