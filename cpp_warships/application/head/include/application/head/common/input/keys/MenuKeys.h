#pragma once

#include <application/head/common/input/InputKey.h>

namespace cpp_warships::head::common {
    class PresentationContext;
}

namespace cpp_warships::head::common::input::keys {
    /** @brief Every binding on the menu works from the context and nothing else. */
    class MenuKey : public InputKey {
    public:
        explicit MenuKey(PresentationContext& context) noexcept;

    protected:
        PresentationContext& context_;
    };

    /** @brief Widens or narrows the board the next match will be played on. */
    class ResizeBoardKey final : public MenuKey {
    public:
        using MenuKey::MenuKey;

        [[nodiscard]] bool matches(const Keystroke& stroke) const override;
        [[nodiscard]] std::optional<model::events::GameEvent> interpret(
            const Keystroke& stroke
        ) override;
    };

    /** @brief Dresses everything in the next palette. */
    class CycleThemeKey final : public MenuKey {
    public:
        using MenuKey::MenuKey;

        [[nodiscard]] bool matches(const Keystroke& stroke) const override;
        [[nodiscard]] std::optional<model::events::GameEvent> interpret(
            const Keystroke& stroke
        ) override;
    };

    /** @brief Starts a match on the board size the player settled on. */
    class StartMatchKey final : public MenuKey {
    public:
        using MenuKey::MenuKey;

        [[nodiscard]] bool matches(const Keystroke& stroke) const override;
        [[nodiscard]] std::optional<model::events::GameEvent> interpret(
            const Keystroke& stroke
        ) override;
    };

    /** @brief Goes back to the match already in play. */
    class ResumeMatchKey final : public InputKey {
    public:
        [[nodiscard]] bool matches(const Keystroke& stroke) const override;
        [[nodiscard]] std::optional<model::events::GameEvent> interpret(
            const Keystroke& stroke
        ) override;
    };

    /** @brief Opens the list of saved games. */
    class LoadMatchKey final : public InputKey {
    public:
        [[nodiscard]] bool matches(const Keystroke& stroke) const override;
        [[nodiscard]] std::optional<model::events::GameEvent> interpret(
            const Keystroke& stroke
        ) override;
    };

    /** @brief Asks what to call the match in play before putting it away. */
    class SaveAndQuitKey final : public InputKey {
    public:
        [[nodiscard]] bool matches(const Keystroke& stroke) const override;
        [[nodiscard]] std::optional<model::events::GameEvent> interpret(
            const Keystroke& stroke
        ) override;
    };

    /** @brief Stops playing. */
    class QuitKey final : public InputKey {
    public:
        [[nodiscard]] bool matches(const Keystroke& stroke) const override;
        [[nodiscard]] std::optional<model::events::GameEvent> interpret(
            const Keystroke& stroke
        ) override;
    };
}  // namespace cpp_warships::head::common::input::keys
