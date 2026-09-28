#pragma once

#include <application/head/common/ScreenKind.h>
#include <application/head/common/host/Shell.h>
#include <application/head/common/input/Keystroke.h>
#include <application/head/common/render/RendererSet.h>

#include <iosfwd>
#include <optional>

namespace cpp_warships::head {
    /** @brief Hosts the interface as plain console text: one frame printed, one
     * line read back. It names no drawing library at all, which is the point of
     * it.
     */
    class TerminalShell final : public Shell {
       public:
        TerminalShell(std::istream& input, std::ostream& output);

        void run(
            PresentationContext& context,
            RendererSet& renderers,
            EventPipeline& pipeline,
            const SessionFinishedQuery& isFinished
        ) override;

       private:
        /** @brief Prints the screen as it now stands, then the prompt for what
         * to do next. */
        void drawFrame(RendererSet& renderers, ScreenKind screen) const;

        /** @brief Reads one typed command, or nothing at all once the input has
         * run out. */
        [[nodiscard]] std::optional<Keystroke> readKeystroke() const;

        std::istream& input_;
        std::ostream& output_;
    };
}  // namespace cpp_warships::head
