#pragma once

#include <ftxui/component/screen_interactive.hpp>

#include <application/head/Queries.h>
#include <application/head/host/Shell.h>

namespace cpp_warships::head {
    /** @brief Hosts the interface in the terminal: the one place that runs an FTXUI loop.
     *  It owns the terminal screen for as long as it exists, so there is always one to quit. */
    class TuiShell final : public Shell {
    public:
        explicit TuiShell(ThemeQuery theme);

        void run(ScreenNavigator& navigator) override;
        void requestQuit() override;

    private:
        ftxui::ScreenInteractive interactiveScreen_;
        ThemeQuery theme_;
    };
} // namespace cpp_warships::head
