#include <build_shell.h>

#include <algorithm>
#include <iostream>
#include <string_view>
#include <utility>

#include <application/head/host/TerminalShell.h>
#include <application/head/host/TuiShell.h>

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

    std::unique_ptr<head::Shell> buildShell(const ShellKind kind, head::ThemeQuery theme) {
        if (kind == ShellKind::PlainTerminal) {
            return std::make_unique<head::TerminalShell>(std::cin, std::cout);
        }

        return std::make_unique<head::TuiShell>(std::move(theme));
    }
} // namespace cpp_warships::application
