#include <application/flow/errors/FlowExceptions.h>

namespace cpp_warships::flow {
    namespace {
        [[nodiscard]] std::string describe(const MatchPhase phase) {
            switch (phase) {
                case MatchPhase::Placement:
                    return "placement";
                case MatchPhase::Battle:
                    return "battle";
                case MatchPhase::Finished:
                    return "finished";
            }

            return "unknown";
        }

        [[nodiscard]] std::string describe(const Participant participant) {
            return participant == Participant::Player ? "the player" : "the computer";
        }
    } // namespace

    FlowException::FlowException(const std::string& message)
        : WarshipsException(core::ErrorLayer::Flow, message) {}

    WrongPhaseException::WrongPhaseException(const MatchPhase expected, const MatchPhase actual)
        : FlowException(
                  "that belongs to the " + describe(expected) + " phase, but the match is in " +
                  describe(actual)
          )
        , expected_(expected)
        , actual_(actual) {}

    MatchPhase WrongPhaseException::expected() const noexcept {
        return expected_;
    }

    MatchPhase WrongPhaseException::actual() const noexcept {
        return actual_;
    }

    NotYourTurnException::NotYourTurnException(
            const Participant acting,
            const Participant current
    )
        : FlowException(describe(acting) + " tried to act on " + describe(current) + "'s turn")
        , acting_(acting)
        , current_(current) {}

    Participant NotYourTurnException::acting() const noexcept {
        return acting_;
    }

    Participant NotYourTurnException::current() const noexcept {
        return current_;
    }

    FleetIncompleteException::FleetIncompleteException(const int shipsStillToPlace)
        : FlowException(
                  "the fleet is not laid out yet: " + std::to_string(shipsStillToPlace) +
                  " still to place"
          )
        , shipsStillToPlace_(shipsStillToPlace) {}

    int FleetIncompleteException::shipsStillToPlace() const noexcept {
        return shipsStillToPlace_;
    }

    SkillUnavailableException::SkillUnavailableException()
        : FlowException("there is no skill banked to use") {}
} // namespace cpp_warships::flow
