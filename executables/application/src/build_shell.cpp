#include <build_shell.h>

#include <algorithm>
#include <iostream>
#include <string_view>
#include <utility>

#include <game_tui/host/TerminalShell.h>
#include <game_tui/host/TuiShell.h>

namespace cpp_warships::application {
    namespace {
        constexpr std::string_view PLAIN_TERMINAL_ARGUMENT = "--plain";
    } // namespace

    ShellKind shellKindFromArguments(const int argumentCount, const char* const* arguments) {
        const auto isPlainTerminal = [](const char* argument) {
            return argument != nullptr && argument == PLAIN_TERMINAL_ARGUMENT;
        };

        const bool wantsPlainTerminal =
                std::any_of(arguments, arguments + argumentCount, isPlainTerminal);

        return wantsPlainTerminal ? ShellKind::PlainTerminal : ShellKind::InteractiveTerminal;
    }

    std::unique_ptr<game_tui::Shell> buildShell(const ShellKind kind, game_tui::ThemeQuery theme) {
        if (kind == ShellKind::PlainTerminal) {
            return std::make_unique<game_tui::TerminalShell>(std::cin, std::cout);
        }

        return std::make_unique<game_tui::TuiShell>(std::move(theme));
    }
} // namespace cpp_warships::application
