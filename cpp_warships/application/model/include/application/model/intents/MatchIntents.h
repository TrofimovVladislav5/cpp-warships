#pragma once

#include <application/core/Coordinate.h>
#include <application/core/Direction.h>
#include <application/model/intents/GameIntent.h>

#include <optional>

namespace cpp_warships::model::behaviors {
    class MatchBehavior;
}

namespace cpp_warships::model::intents {
    /** @brief Starts a fresh match on a board of the asked-for size. */
    class StartMatchIntent final : public GameIntent {
    public:
        StartMatchIntent(behaviors::MatchBehavior& play, int boardSize) noexcept;

        [[nodiscard]] std::string name() const override;
        [[nodiscard]] IntentResult apply() const override;

    private:
        behaviors::MatchBehavior& play_;
        int boardSize_;
    };

    /** @brief Lays one ship out. */
    class PlaceShipIntent final : public GameIntent {
    public:
        PlaceShipIntent(
            behaviors::MatchBehavior& play,
            core::Coordinate origin,
            core::Direction direction,
            int length
        ) noexcept;

        [[nodiscard]] std::string name() const override;
        [[nodiscard]] IntentResult apply() const override;

    private:
        behaviors::MatchBehavior& play_;
        core::Coordinate origin_;
        core::Direction direction_;
        int length_;
    };

    /** @brief Takes one ship back off the board. */
    class RemoveShipIntent final : public GameIntent {
    public:
        RemoveShipIntent(behaviors::MatchBehavior& play, core::Coordinate coordinate) noexcept;

        [[nodiscard]] std::string name() const override;
        [[nodiscard]] IntentResult apply() const override;

    private:
        behaviors::MatchBehavior& play_;
        core::Coordinate coordinate_;
    };

    /** @brief Lays the whole fleet out at random. */
    class ShuffleFleetIntent final : public GameIntent {
    public:
        explicit ShuffleFleetIntent(behaviors::MatchBehavior& play) noexcept;

        [[nodiscard]] std::string name() const override;
        [[nodiscard]] IntentResult apply() const override;

    private:
        behaviors::MatchBehavior& play_;
    };

    /** @brief Stops laying out and opens fire. */
    class BeginBattleIntent final : public GameIntent {
    public:
        explicit BeginBattleIntent(behaviors::MatchBehavior& play) noexcept;

        [[nodiscard]] std::string name() const override;
        [[nodiscard]] IntentResult apply() const override;

    private:
        behaviors::MatchBehavior& play_;
    };

    /** @brief Fires on one cell of the enemy's waters. */
    class FireAtIntent final : public GameIntent {
    public:
        FireAtIntent(behaviors::MatchBehavior& play, core::Coordinate coordinate) noexcept;

        [[nodiscard]] std::string name() const override;
        [[nodiscard]] IntentResult apply() const override;

    private:
        behaviors::MatchBehavior& play_;
        core::Coordinate coordinate_;
    };

    /** @brief Spends the next banked skill, on @p target when the skill wants one. */
    class UseSkillIntent final : public GameIntent {
    public:
        UseSkillIntent(
            behaviors::MatchBehavior& play,
            std::optional<core::Coordinate> target
        ) noexcept;

        [[nodiscard]] std::string name() const override;
        [[nodiscard]] IntentResult apply() const override;

    private:
        behaviors::MatchBehavior& play_;
        std::optional<core::Coordinate> target_;
    };
}  // namespace cpp_warships::model::intents
