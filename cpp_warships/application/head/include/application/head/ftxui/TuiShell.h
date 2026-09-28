#pragma once

#include <application/head/common/Queries.h>
#include <application/head/common/host/Shell.h>

#include <ftxui/component/screen_interactive.hpp>

namespace cpp_warships::head {
    /** @brief Hosts the interface in the terminal: the one place that runs an
     * FTXUI loop. It owns the terminal screen for as long as it exists, so
     * there is always one to quit. */
    class TuiShell final : public Shell {
       public:
        explicit TuiShell(ThemeQuery theme);

        void run(
            PresentationContext& context,
            RendererSet& renderers,
            EventPipeline& pipeline,
            const SessionFinishedQuery& isFinished
        ) override;

       private:
        ftxui::ScreenInteractive interactiveScreen_;
        ThemeQuery theme_;
    };
}  // namespace cpp_warships::head
