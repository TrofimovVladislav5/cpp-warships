#pragma once

#include <application/core/Coordinate.h>
#include <application/core/Direction.h>

#include <optional>
#include <string>
#include <variant>

namespace cpp_warships::model::events {
    /** @brief The player asked for a new match on a board of @p boardSize a side. */
    struct MatchStartRequested {
        int boardSize = 10;
    };

    /** @brief The player asked to go back to the match already in play. */
    struct MatchResumeRequested {};

    /** @brief The player asked to see what has been saved. */
    struct SaveBrowserRequested {};

    /** @brief The player asked for the save called @p name to be picked back up. */
    struct MatchLoadRequested {
        std::string name;
    };

    /** @brief The player asked for the save called @p name to be thrown away. */
    struct SaveDeleteRequested {
        std::string name;
    };

    /** @brief The player asked to stop playing. */
    struct SessionQuitRequested {};

    /** @brief The player asked to give the match in play a name before putting it away. */
    struct SaveNamingRequested {};

    /** @brief The player named the match @p name and asked to put it away and stop. */
    struct MatchSaveAndQuitRequested {
        std::string name;
    };

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
        SaveBrowserRequested,
        MatchLoadRequested,
        SaveDeleteRequested,
        SessionQuitRequested,
        SaveNamingRequested,
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
