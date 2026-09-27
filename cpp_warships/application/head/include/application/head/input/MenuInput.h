#pragma once

#include <application/head/PresentationContext.h>
#include <application/head/input/ScreenInput.h>

namespace cpp_warships::head {
    /** @brief The menu read in the game's terms. Board size and palette are settled here
     *  and now, because neither is the game's business until a match is actually asked for. */
    class MenuInput final : public ScreenInput {
    public:
        /** @brief Reads onto @p context, which must outlive it. */
        explicit MenuInput(PresentationContext& context) noexcept;

        [[nodiscard]] std::optional<model::GameEvent> interpret(const Keystroke& stroke) override;

    private:
        void resizeBoard(int step);
        void cycleTheme();

        PresentationContext& context_;
    };
} // namespace cpp_warships::head
