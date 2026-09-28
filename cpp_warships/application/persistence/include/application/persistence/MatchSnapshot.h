#pragma once

#include <application/core/Board.h>
#include <application/core/MatchSettings.h>
#include <application/flow/AiOpponent.h>
#include <application/flow/MatchEvent.h>
#include <application/flow/MatchPhase.h>
#include <application/flow/Participant.h>
#include <application/flow/SkillKind.h>
#include <serialization/ISerializable.h>

#include <deque>

namespace cpp_warships::flow {
    class Match;
}

namespace cpp_warships::persistence {
    inline char MATCH_SNAPSHOT_NAME[] = "MatchSnapshot";

    /** @brief A whole match frozen into plain data, ready to be written out or read back. */
    class MatchSnapshot final : public serialization::ISerializable<MATCH_SNAPSHOT_NAME> {
    public:
        MatchSnapshot(
            core::MatchSettings settings,
            core::Board playerBoard,
            core::Board computerBoard,
            std::deque<flow::SkillKind> bankedSkills,
            int roundNumber,
            flow::MatchPhase phase,
            flow::Participant currentTurn,
            bool isDoubleDamageArmed,
            flow::AiMemory opponentMemory,
            flow::MatchEventLog journal
        );

        /** @brief Captures @p match exactly as it stands. */
        /** @brief Everything about @p match worth keeping, together with @p
         * journal, which the match itself has already forgotten. */
        [[nodiscard]] static MatchSnapshot capture(
            const flow::Match& match,
            const flow::MatchEventLog& journal
        );

        /** @brief Rebuilds a match from this snapshot, drawing new randomness from @p engine. */
        [[nodiscard]] flow::Match restore(flow::RandomEngine& randomEngine) const;

        [[nodiscard]] const core::MatchSettings& settings() const noexcept;
        [[nodiscard]] const core::Board& playerBoard() const noexcept;
        [[nodiscard]] const core::Board& computerBoard() const noexcept;
        [[nodiscard]] const std::deque<flow::SkillKind>& bankedSkills() const noexcept;
        [[nodiscard]] int roundNumber() const noexcept;
        [[nodiscard]] flow::MatchPhase phase() const noexcept;
        [[nodiscard]] flow::Participant currentTurn() const noexcept;
        [[nodiscard]] bool isDoubleDamageArmed() const noexcept;
        [[nodiscard]] const flow::AiMemory& opponentMemory() const noexcept;

        /** @brief What has happened so far, in the events the match reported. */
        [[nodiscard]] const flow::MatchEventLog& journal() const noexcept;

    private:
        core::MatchSettings settings_;
        core::Board playerBoard_;
        core::Board computerBoard_;
        std::deque<flow::SkillKind> bankedSkills_;
        int roundNumber_;
        flow::MatchPhase phase_;
        flow::Participant currentTurn_;
        bool isDoubleDamageArmed_;
        flow::AiMemory opponentMemory_;
        flow::MatchEventLog journal_;
    };
}  // namespace cpp_warships::persistence
