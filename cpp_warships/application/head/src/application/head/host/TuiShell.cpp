#include <application/head/host/TuiShell.h>

#include <utility>

#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/terminal.hpp>

#include <application/head/PresentationContext.h>
#include <application/head/input/EventPipeline.h>
#include <application/head/views/RendererSet.h>
#include <application/head/views/ftxui_bridge/FtxuiPalette.h>
#include <application/head/views/ftxui_bridge/FtxuiView.h>

namespace cpp_warships::head {
    TuiShell::TuiShell(ThemeQuery theme)
        : interactiveScreen_(ftxui::ScreenInteractive::Fullscreen())
        , theme_(std::move(theme)) {}

    void TuiShell::run(
            PresentationContext& context,
            RendererSet& renderers,
            EventPipeline& pipeline,
            const SessionFinishedQuery& isFinished
    ) {
        const auto renderActiveScreen = [&context, &renderers] {
            const auto [dimx, dimy] = ftxui::Terminal::Size();
            const Frame frame = renderers.render(context.currentScreen(), dimx, dimy);

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
} // namespace cpp_warships::head
