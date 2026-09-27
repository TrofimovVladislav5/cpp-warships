#pragma once

#include <application/head/PresentationContext.h>
#include <application/head/input/ScreenInput.h>

namespace cpp_warships::head {
    /** @brief The battle read in the game's terms. Taking aim and reading back through the
     *  log never trouble the game; only firing and spending a skill do. */
    class BattleInput final : public ScreenInput {
    public:
        /** @brief Reads onto @p context, which must outlive it. */
        explicit BattleInput(PresentationContext& context) noexcept;

        [[nodiscard]] std::optional<model::GameEvent> interpret(const Keystroke& stroke) override;

    private:
        [[nodiscard]] std::optional<model::GameEvent> interpretPointer(const Keystroke& stroke);

        void moveAim(const Keystroke& stroke);
        void scrollLog(int step);

        PresentationContext& context_;
    };
} // namespace cpp_warships::head
