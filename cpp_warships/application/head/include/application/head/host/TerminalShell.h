#pragma once

#include <iosfwd>
#include <optional>

#include <application/head/host/Shell.h>
#include <application/head/input/Keystroke.h>

namespace cpp_warships::head {
    class GameView;

    /** @brief Hosts the interface as plain console text: one frame printed, one line read back.
     *  It names no drawing library at all, which is the point of it. */
    class TerminalShell final : public Shell {
    public:
        TerminalShell(std::istream& input, std::ostream& output);

        void run(ScreenNavigator& navigator) override;
        void requestQuit() override;

    private:
        /** @brief Prints @p view as it now stands, followed by the prompt for what to do next. */
        void drawFrame(GameView& view) const;

        /** @brief Reads one typed command, or nothing at all once the input has run out. */
        [[nodiscard]] std::optional<Keystroke> readKeystroke() const;

        std::istream& input_;
        std::ostream& output_;
        bool isRunning_ = false;
    };
} // namespace cpp_warships::head
