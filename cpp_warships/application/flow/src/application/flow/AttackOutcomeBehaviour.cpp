#include <application/core/Board.h>
#include <application/flow/AiOpponent.h>
#include <application/flow/AttackOutcomeBehaviour.h>

#include <unordered_map>

namespace cpp_warships::flow {
    namespace {
        /** @brief A shot that found open water: the turn passes to the
         * opponent. */
        class MissBehaviour final : public AttackOutcomeBehaviour {
        public:
            [[nodiscard]] bool keepsTurn() const override {
                return false;
            }

            [[nodiscard]] bool isShotSpent() const override {
                return true;
            }

            [[nodiscard]] bool grantsSkill() const override {
                return false;
            }

            [[nodiscard]] MatchEventKind eventKind() const override {
                return MatchEventKind::ShotMissed;
            }

            void updateHunt(AiOpponent&, core::Coordinate, const core::Board&) const override {}
        };

        /** @brief A shot that wounded a ship without finishing it: the shooter fires again. */
        class HitBehaviour final : public AttackOutcomeBehaviour {
        public:
            [[nodiscard]] bool keepsTurn() const override {
                return true;
            }

            [[nodiscard]] bool isShotSpent() const override {
                return true;
            }

            [[nodiscard]] bool grantsSkill() const override {
                return false;
            }

            [[nodiscard]] MatchEventKind eventKind() const override {
                return MatchEventKind::ShipDamaged;
            }

            void updateHunt(
                AiOpponent& opponent,
                core::Coordinate coordinate,
                const core::Board&
            ) const override {
                opponent.registerHit(coordinate);
            }
        };

        /** @brief A shot that sank a ship: the shooter fires again and earns a skill. */
        class SunkBehaviour final : public AttackOutcomeBehaviour {
        public:
            [[nodiscard]] bool keepsTurn() const override {
                return true;
            }

            [[nodiscard]] bool isShotSpent() const override {
                return true;
            }

            [[nodiscard]] bool grantsSkill() const override {
                return true;
            }

            [[nodiscard]] MatchEventKind eventKind() const override {
                return MatchEventKind::ShipSunk;
            }

            void updateHunt(
                AiOpponent& opponent,
                core::Coordinate coordinate,
                const core::Board& board
            ) const override {
                opponent.registerHit(coordinate);
                opponent.finishHunt(board);
            }
        };

        /** @brief A shot that was never taken, off the board or at a resolved cell. It costs
         * nothing: the turn stays and any armed bonus is still waiting. */
        class RejectedBehaviour final : public AttackOutcomeBehaviour {
        public:
            [[nodiscard]] bool keepsTurn() const override {
                return true;
            }

            [[nodiscard]] bool isShotSpent() const override {
                return false;
            }

            [[nodiscard]] bool grantsSkill() const override {
                return false;
            }

            [[nodiscard]] MatchEventKind eventKind() const override {
                return MatchEventKind::ShotRejected;
            }

            void updateHunt(AiOpponent&, core::Coordinate, const core::Board&) const override {}
        };

        const MissBehaviour MISS_BEHAVIOUR;
        const HitBehaviour HIT_BEHAVIOUR;
        const SunkBehaviour SUNK_BEHAVIOUR;
        const RejectedBehaviour REJECTED_BEHAVIOUR;

        const std::unordered_map<core::AttackOutcome, const AttackOutcomeBehaviour*>
            BEHAVIOUR_BY_OUTCOME{
                {core::AttackOutcome::Miss, &MISS_BEHAVIOUR},
                {core::AttackOutcome::Hit, &HIT_BEHAVIOUR},
                {core::AttackOutcome::Sunk, &SUNK_BEHAVIOUR},
                {core::AttackOutcome::AlreadyAttacked, &REJECTED_BEHAVIOUR},
                {core::AttackOutcome::OutOfBounds, &REJECTED_BEHAVIOUR}
            };
    }  // namespace

    const AttackOutcomeBehaviour& behaviourFor(core::AttackOutcome outcome) {
        return *BEHAVIOUR_BY_OUTCOME.at(outcome);
    }
}  // namespace cpp_warships::flow
