#pragma once

#include <deque>

#include <application/core/Board.h>
#include <application/core/MatchSettings.h>
#include <application/flow/AiOpponent.h>
#include <application/flow/Match.h>
#include <application/flow/Participant.h>
#include <application/flow/SkillKind.h>
#include <serialization/ISerializable.h>

namespace cpp_warships::persistence {
    inline char MATCH_SNAPSHOT_NAME[] = "MatchSnapshot";

    /** @brief A whole match frozen into plain data, ready to be written out or read back.
     *  Only this layer knows about serialising, which is what keeps the rules free of it. */
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
                flow::AiMemory opponentMemory
        );

        /** @brief Captures @p match exactly as it stands. */
        [[nodiscard]] static MatchSnapshot capture(const flow::Match& match);

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
    };
} // namespace cpp_warships::persistence
