#include <application/head/common/PresentationContext.h>
#include <application/head/common/input/EventPipeline.h>
#include <application/head/common/render/RendererSet.h>
#include <application/head/tui/FtxuiPalette.h>
#include <application/head/tui/FtxuiView.h>
#include <application/head/tui/TuiShell.h>

#include <cstdio>
#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/terminal.hpp>
#include <iostream>
#include <utility>

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

namespace cpp_warships::head::tui {
    namespace {
        /** @brief Whether standard output is a terminal rather than a pipe. */
        bool isOutputTerminal() {
#ifdef _WIN32
            return _isatty(_fileno(stdout)) != 0;
#else
            return isatty(STDOUT_FILENO) != 0;
#endif
        }

        /** @brief Blanks the alternate screen before the library switches into
         * it. */
        void blankAlternateScreen() {
            if (!isOutputTerminal()) {
                return;
            }

            std::cout << "\033[?1049h\033[2J\033[H" << std::flush;
        }
    }  // namespace

    TuiShell::TuiShell(common::ThemeQuery theme)
        : interactiveScreen_(ftxui::ScreenInteractive::Fullscreen())
        , theme_(std::move(theme)) {}

    void TuiShell::run(
        common::PresentationContext& context,
        common::render::RendererSet& renderers,
        common::input::EventPipeline& pipeline,
        const common::host::SessionFinishedQuery& isFinished
    ) {
        blankAlternateScreen();

        const auto renderActiveScreen = [&context, &renderers] {
            const auto [dimx, dimy] = ftxui::Terminal::Size();
            const common::render::Frame frame =
                renderers.render(context.currentScreen(), dimx, dimy);

            return elementOfFrame(frame) | bgcolor(context.theme().background);
        };

        const auto routeEvent = [this, &pipeline, &isFinished](const ftxui::Event& event) {
            pipeline.offer(keystrokeOf(event));
            const bool isClaimed = pipeline.settle();

            if (isFinished()) {
                interactiveScreen_.Exit();
            }

            return isClaimed;
        };

        interactiveScreen_.Loop(ftxui::CatchEvent(ftxui::Renderer(renderActiveScreen), routeEvent));
    }
}  // namespace cpp_warships::head::tui
