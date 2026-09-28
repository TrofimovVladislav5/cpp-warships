#pragma once

#include <application/head/common/ScreenKind.h>
#include <application/head/common/host/Shell.h>
#include <application/head/common/input/Keystroke.h>

#include <iosfwd>
#include <optional>

namespace cpp_warships::head::common::render {
    class RendererSet;
}

namespace cpp_warships::head::plain {
    /** @brief Hosts the interface as plain console text: one frame printed, one line read back. */
    class TerminalShell final : public common::host::Shell {
    public:
        TerminalShell(std::istream& input, std::ostream& output);

        void run(
            common::PresentationContext& context,
            common::render::RendererSet& renderers,
            common::input::EventPipeline& pipeline,
            const common::host::SessionFinishedQuery& isFinished
        ) override;

    private:
        /** @brief Prints the screen as it now stands, then the prompt for what
         * to do next. */
        void drawFrame(common::render::RendererSet& renderers, common::ScreenKind screen) const;

        /** @brief Reads one typed command, or nothing at all once the input has
         * run out. */
        [[nodiscard]] std::optional<common::input::Keystroke> readKeystroke() const;

        std::istream& input_;
        std::ostream& output_;
    };
}  // namespace cpp_warships::head::plain
