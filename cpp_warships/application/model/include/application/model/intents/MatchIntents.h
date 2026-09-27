#pragma once

#include <optional>

#include <application/core/Coordinate.h>
#include <application/core/Direction.h>
#include <application/model/behaviors/MatchBehavior.h>
#include <application/model/intents/GameIntent.h>

namespace cpp_warships::model {
    /** @brief Starts a fresh match on a board of the asked-for size. */
    class StartMatchIntent final : public GameIntent {
    public:
        StartMatchIntent(MatchBehavior& play, int boardSize) noexcept;

        [[nodiscard]] std::string name() const override;
        [[nodiscard]] IntentResult apply() const override;

    private:
        MatchBehavior& play_;
        int boardSize_;
    };

    /** @brief Lays one ship out. */
    class PlaceShipIntent final : public GameIntent {
    public:
        PlaceShipIntent(
                MatchBehavior& play,
                core::Coordinate origin,
                core::Direction direction,
                int length
        ) noexcept;

        [[nodiscard]] std::string name() const override;
        [[nodiscard]] IntentResult apply() const override;

    private:
        MatchBehavior& play_;
        core::Coordinate origin_;
        core::Direction direction_;
        int length_;
    };

    /** @brief Takes one ship back off the board. */
    class RemoveShipIntent final : public GameIntent {
    public:
        RemoveShipIntent(MatchBehavior& play, core::Coordinate coordinate) noexcept;

        [[nodiscard]] std::string name() const override;
        [[nodiscard]] IntentResult apply() const override;

    private:
        MatchBehavior& play_;
        core::Coordinate coordinate_;
    };

    /** @brief Lays the whole fleet out at random. */
    class ShuffleFleetIntent final : public GameIntent {
    public:
        explicit ShuffleFleetIntent(MatchBehavior& play) noexcept;

        [[nodiscard]] std::string name() const override;
        [[nodiscard]] IntentResult apply() const override;

    private:
        MatchBehavior& play_;
    };

    /** @brief Stops laying out and opens fire. */
    class BeginBattleIntent final : public GameIntent {
    public:
        explicit BeginBattleIntent(MatchBehavior& play) noexcept;

        [[nodiscard]] std::string name() const override;
        [[nodiscard]] IntentResult apply() const override;

    private:
        MatchBehavior& play_;
    };

    /** @brief Fires on one cell of the enemy's waters. */
    class FireAtIntent final : public GameIntent {
    public:
        FireAtIntent(MatchBehavior& play, core::Coordinate coordinate) noexcept;

        [[nodiscard]] std::string name() const override;
        [[nodiscard]] IntentResult apply() const override;

    private:
        MatchBehavior& play_;
        core::Coordinate coordinate_;
    };

    /** @brief Spends the next banked skill, on @p target when the skill wants one. */
    class UseSkillIntent final : public GameIntent {
    public:
        UseSkillIntent(MatchBehavior& play, std::optional<core::Coordinate> target) noexcept;

        [[nodiscard]] std::string name() const override;
        [[nodiscard]] IntentResult apply() const override;

    private:
        MatchBehavior& play_;
        std::optional<core::Coordinate> target_;
    };
} // namespace cpp_warships::model
