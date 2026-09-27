#include <game_tui/host/TuiShell.h>

#include <utility>

#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/terminal.hpp>

#include <game_tui/screens/ScreenNavigator.h>
#include <game_tui/views/ftxui_bridge/FtxuiPalette.h>
#include <game_tui/views/ftxui_bridge/FtxuiView.h>

namespace cpp_warships::game_tui {
    TuiShell::TuiShell(ThemeQuery theme)
        : interactiveScreen_(ftxui::ScreenInteractive::Fullscreen())
        , theme_(std::move(theme)) {}

    void TuiShell::run(ScreenNavigator& navigator) {
        const auto renderActiveScreen = [this, &navigator] {
            const auto [dimx, dimy] = ftxui::Terminal::Size();
            const Frame frame = navigator.activeScreen().view().render(dimx, dimy);

            return elementOfFrame(frame) | bgcolor(theme_().background);
        };

        const auto routeEvent = [&navigator](const ftxui::Event& event) {
            return navigator.activeScreen().handleEvent(keystrokeOf(event));
        };

        interactiveScreen_.Loop(ftxui::CatchEvent(ftxui::Renderer(renderActiveScreen), routeEvent));
    }

    void TuiShell::requestQuit() {
        interactiveScreen_.Exit();
    }
} // namespace cpp_warships::game_tui
