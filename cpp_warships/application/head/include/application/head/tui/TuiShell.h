#pragma once

#include <application/head/common/Queries.h>
#include <application/head/common/host/Shell.h>

#include <ftxui/component/screen_interactive.hpp>

namespace cpp_warships::head::tui {
    /** @brief Hosts the interface in the terminal: the one place that runs an FTXUI loop. */
    class TuiShell final : public common::host::Shell {
    public:
        explicit TuiShell(common::ThemeQuery theme);

        void run(
            common::PresentationContext& context,
            common::render::RendererSet& renderers,
            common::input::EventPipeline& pipeline,
            const common::host::SessionFinishedQuery& isFinished
        ) override;

    private:
        ftxui::ScreenInteractive interactiveScreen_;
        common::ThemeQuery theme_;
    };
}  // namespace cpp_warships::head::tui
