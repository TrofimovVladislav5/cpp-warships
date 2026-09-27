#pragma once

#include <string>

#include <application/core/errors/WarshipsException.h>
#include <application/flow/MatchPhase.h>
#include <application/flow/Participant.h>

namespace cpp_warships::flow {
    /** @brief A match asked to do something it is not in a position to do.
     *  The rules were not broken here: the moment was wrong, not the move. */
    class FlowException : public core::WarshipsException {
    protected:
        explicit FlowException(const std::string& message);
    };

    /** @brief Something was asked of a match that only another phase allows. */
    class WrongPhaseException final : public FlowException {
    public:
        WrongPhaseException(MatchPhase expected, MatchPhase actual);

        [[nodiscard]] MatchPhase expected() const noexcept;
        [[nodiscard]] MatchPhase actual() const noexcept;

    private:
        MatchPhase expected_;
        MatchPhase actual_;
    };

    /** @brief A participant tried to act while it was not their turn. */
    class NotYourTurnException final : public FlowException {
    public:
        NotYourTurnException(Participant acting, Participant current);

        [[nodiscard]] Participant acting() const noexcept;
        [[nodiscard]] Participant current() const noexcept;

    private:
        Participant acting_;
        Participant current_;
    };

    /** @brief Battle was called for while ships were still waiting to be placed. */
    class FleetIncompleteException final : public FlowException {
    public:
        explicit FleetIncompleteException(int shipsStillToPlace);

        [[nodiscard]] int shipsStillToPlace() const noexcept;

    private:
        int shipsStillToPlace_;
    };

    /** @brief A skill was called on when the bank had none to give. */
    class SkillUnavailableException final : public FlowException {
    public:
        SkillUnavailableException();
    };
} // namespace cpp_warships::flow
