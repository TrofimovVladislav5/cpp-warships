#include <application/persistence/MatchSnapshot.h>

#include <utility>

namespace cpp_warships::persistence {
    MatchSnapshot::MatchSnapshot(
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
    )
        : settings_(std::move(settings)),
          playerBoard_(std::move(playerBoard)),
          computerBoard_(std::move(computerBoard)),
          bankedSkills_(std::move(bankedSkills)),
          roundNumber_(roundNumber),
          phase_(phase),
          currentTurn_(currentTurn),
          isDoubleDamageArmed_(isDoubleDamageArmed),
          opponentMemory_(std::move(opponentMemory)),
          journal_(std::move(journal)) {
    }

    MatchSnapshot
    MatchSnapshot::capture(const flow::Match& match, const flow::MatchEventLog& journal) {
        return MatchSnapshot{
            match.settings(),
            match.playerBoard(),
            match.computerBoard(),
            match.skills().pending(),
            match.roundNumber(),
            match.phase(),
            match.currentTurn(),
            match.isDoubleDamageArmed(),
            match.opponentMemory(),
            journal
        };
    }

    flow::Match MatchSnapshot::restore(flow::RandomEngine& randomEngine) const {
        flow::MatchRestoreState state{
            .playerBoard = playerBoard_,
            .computerBoard = computerBoard_,
            .bankedSkills = bankedSkills_,
            .roundNumber = roundNumber_,
            .phase = phase_,
            .currentTurn = currentTurn_,
            .isDoubleDamageArmed = isDoubleDamageArmed_,
            .opponentMemory = opponentMemory_
        };

        return flow::Match{settings_, randomEngine, std::move(state)};
    }

    const core::MatchSettings& MatchSnapshot::settings() const noexcept {
        return settings_;
    }

    const core::Board& MatchSnapshot::playerBoard() const noexcept {
        return playerBoard_;
    }

    const core::Board& MatchSnapshot::computerBoard() const noexcept {
        return computerBoard_;
    }

    const std::deque<flow::SkillKind>& MatchSnapshot::bankedSkills() const noexcept {
        return bankedSkills_;
    }

    int MatchSnapshot::roundNumber() const noexcept {
        return roundNumber_;
    }

    flow::MatchPhase MatchSnapshot::phase() const noexcept {
        return phase_;
    }

    flow::Participant MatchSnapshot::currentTurn() const noexcept {
        return currentTurn_;
    }

    bool MatchSnapshot::isDoubleDamageArmed() const noexcept {
        return isDoubleDamageArmed_;
    }

    const flow::AiMemory& MatchSnapshot::opponentMemory() const noexcept {
        return opponentMemory_;
    }

    const flow::MatchEventLog& MatchSnapshot::journal() const noexcept {
        return journal_;
    }
}  // namespace cpp_warships::persistence
