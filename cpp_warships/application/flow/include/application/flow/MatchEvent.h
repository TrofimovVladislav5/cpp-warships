#pragma once

#include <application/core/Coordinate.h>
#include <application/flow/Participant.h>
#include <application/flow/SkillKind.h>

#include <optional>
#include <vector>

namespace cpp_warships::flow {
    /** @brief Something that happened during a match, in the order it happened. */
    enum class MatchEventKind {
        ShotMissed,
        ShipDamaged,
        ShipSunk,
        ShotRejected,
        SkillGranted,
        DoubleDamageArmed,
        AreaScanned,
        RoundWon,
        MatchLost,
        TurnPassed,
    };

    /** @brief One entry in a match's history. The interface renders these and
     * animations replay them; the match draws nothing. */
    struct MatchEvent {
        MatchEventKind kind;
        Participant actor = Participant::Player;
        std::optional<core::Coordinate> coordinate = std::nullopt;
        std::optional<SkillKind> skill = std::nullopt;
        /** @brief For AreaScanned: whether the scanned area held a ship. */
        bool scanFoundShip = false;
    };

    using MatchEventLog = std::vector<MatchEvent>;
}  // namespace cpp_warships::flow
