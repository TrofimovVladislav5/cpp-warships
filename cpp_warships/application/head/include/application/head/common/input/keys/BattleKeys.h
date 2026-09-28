#pragma once

#include <application/head/common/input/InputKey.h>
#include <application/head/common/input/keys/MoveCursorKey.h>

namespace cpp_warships::head::common {
    class PresentationContext;
}

namespace cpp_warships::head::common::input::keys {
    /** @brief Scrolls the log by @p step, stopping at either end of what there is. */
    void scrollLog(PresentationContext& context, int step);

    /** @brief Every binding here works from the context and nothing else. */
    class BattleKey : public InputKey {
    public:
        explicit BattleKey(PresentationContext& context) noexcept;

    protected:
        /** @brief The cell the pointer is over, when it is over the enemy's waters. */
        [[nodiscard]] std::optional<core::Coordinate> cellUnderPointer(
            const Keystroke& stroke
        ) const;

        PresentationContext& context_;
    };

    /** @brief Walks the aim around the enemy's waters. */
    class MoveBattleAimKey final : public MoveCursorKey {
    public:
        using MoveCursorKey::MoveCursorKey;

    protected:
        [[nodiscard]] core::Coordinate& cursor() override;
        [[nodiscard]] const core::Board& board() const override;
    };

    /** @brief Reads further back through the log. */
    class ScrollLogBackKey final : public BattleKey {
    public:
        using BattleKey::BattleKey;

        [[nodiscard]] bool matches(const Keystroke& stroke) const override;
        [[nodiscard]] std::optional<model::events::GameEvent> interpret(
            const Keystroke& stroke
        ) override;
    };

    /** @brief Reads back towards the newest of the log. */
    class ScrollLogForwardKey final : public BattleKey {
    public:
        using BattleKey::BattleKey;

        [[nodiscard]] bool matches(const Keystroke& stroke) const override;
        [[nodiscard]] std::optional<model::events::GameEvent> interpret(
            const Keystroke& stroke
        ) override;
    };

    /** @brief Fires on wherever the aim rests. */
    class FireKey final : public BattleKey {
    public:
        using BattleKey::BattleKey;

        [[nodiscard]] bool matches(const Keystroke& stroke) const override;
        [[nodiscard]] std::optional<model::events::GameEvent> interpret(
            const Keystroke& stroke
        ) override;
    };

    /** @brief Spends the next banked skill, offering wherever the aim rests. */
    class UseSkillKey final : public BattleKey {
    public:
        using BattleKey::BattleKey;

        [[nodiscard]] bool matches(const Keystroke& stroke) const override;
        [[nodiscard]] std::optional<model::events::GameEvent> interpret(
            const Keystroke& stroke
        ) override;
    };

    /** @brief Scrolls the log by rolling the wheel over it. */
    class ScrollLogWithWheelKey final : public BattleKey {
    public:
        using BattleKey::BattleKey;

        [[nodiscard]] bool matches(const Keystroke& stroke) const override;
        [[nodiscard]] std::optional<model::events::GameEvent> interpret(
            const Keystroke& stroke
        ) override;
    };

    /** @brief Fires on wherever the pointer was pressed. */
    class FireWithPointerKey final : public BattleKey {
    public:
        using BattleKey::BattleKey;

        [[nodiscard]] bool matches(const Keystroke& stroke) const override;
        [[nodiscard]] std::optional<model::events::GameEvent> interpret(
            const Keystroke& stroke
        ) override;
    };

    /** @brief Moves the aim to wherever the pointer is, asking the game for nothing. */
    class AimWithPointerAtEnemyKey final : public BattleKey {
    public:
        using BattleKey::BattleKey;

        [[nodiscard]] bool matches(const Keystroke& stroke) const override;
        [[nodiscard]] std::optional<model::events::GameEvent> interpret(
            const Keystroke& stroke
        ) override;
    };
}  // namespace cpp_warships::head::common::input::keys
