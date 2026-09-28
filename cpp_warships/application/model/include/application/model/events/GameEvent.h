#pragma once

#include <application/core/Coordinate.h>
#include <application/core/Direction.h>

#include <optional>
#include <variant>

namespace cpp_warships::model::events {
    /** @brief The player asked for a new match on a board of @p boardSize a side. */
    struct MatchStartRequested {
        int boardSize = 10;
    };

    /** @brief The player asked to go back to the match already in play. */
    struct MatchResumeRequested {};

    /** @brief The player asked for the saved match to be picked back up. */
    struct MatchLoadRequested {};

    /** @brief The player asked to stop playing. */
    struct SessionQuitRequested {};

    /** @brief The player asked to put the match away and stop, as one thing. */
    struct MatchSaveAndQuitRequested {};

    /** @brief The player asked to step back out to the menu. */
    struct MenuReturnRequested {};

    /** @brief The player asked for a ship of @p length to be laid out. */
    struct ShipPlacementRequested {
        core::Coordinate origin;
        core::Direction direction = core::Direction::Horizontal;
        int length = 0;
    };

    /** @brief The player asked for whichever ship covers @p coordinate to be
     * taken back. */
    struct ShipRemovalRequested {
        core::Coordinate coordinate;
    };

    /** @brief The player asked for the fleet to be laid out for them. */
    struct FleetShuffleRequested {};

    /** @brief The player asked to stop laying out and open fire. */
    struct BattleBeginRequested {};

    /** @brief The player asked to fire on @p target. */
    struct ShotRequested {
        core::Coordinate target;
    };

    /** @brief The player asked to spend the next banked skill. */
    struct SkillUseRequested {
        std::optional<core::Coordinate> aim;
    };

    /** @brief Something the player asked for, in the game's own terms rather than in keys. */
    using GameEvent = std::variant<
        MatchStartRequested,
        MatchResumeRequested,
        MatchLoadRequested,
        SessionQuitRequested,
        MatchSaveAndQuitRequested,
        MenuReturnRequested,
        ShipPlacementRequested,
        ShipRemovalRequested,
        FleetShuffleRequested,
        BattleBeginRequested,
        ShotRequested,
        SkillUseRequested>;

    /** @brief Whether @p event is of the given kind, as a question worth one word. */
    template <typename TEvent>
    [[nodiscard]] bool isKind(const GameEvent& event) noexcept {
        return std::holds_alternative<TEvent>(event);
    }
}  // namespace cpp_warships::model::events
