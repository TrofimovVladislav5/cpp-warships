#include <application/core/Board.h>
#include <application/core/Coordinate.h>
#include <application/core/Direction.h>
#include <application/core/Outcomes.h>
#include <application/flow/AiOpponent.h>
#include <application/flow/AttackOutcomeBehaviour.h>
#include <application/flow/MatchEvent.h>
#include <application/flow/RandomEngine.h>
#include <gtest/gtest.h>

namespace cpp_warships::flow {
    TEST(AttackOutcomeBehaviourTests, AMissEndsTheTurnAndSpendsTheShot) {
        const AttackOutcomeBehaviour& behaviour = behaviourFor(core::AttackOutcome::Miss);

        EXPECT_FALSE(behaviour.keepsTurn());
        EXPECT_TRUE(behaviour.isShotSpent());
        EXPECT_FALSE(behaviour.grantsSkill());
        EXPECT_EQ(behaviour.eventKind(), MatchEventKind::ShotMissed);
    }

    TEST(AttackOutcomeBehaviourTests, AHitKeepsTheTurnWithoutEarningASkill) {
        const AttackOutcomeBehaviour& behaviour = behaviourFor(core::AttackOutcome::Hit);

        EXPECT_TRUE(behaviour.keepsTurn());
        EXPECT_TRUE(behaviour.isShotSpent());
        EXPECT_FALSE(behaviour.grantsSkill());
        EXPECT_EQ(behaviour.eventKind(), MatchEventKind::ShipDamaged);
    }

    TEST(AttackOutcomeBehaviourTests, SinkingKeepsTheTurnAndEarnsASkill) {
        const AttackOutcomeBehaviour& behaviour = behaviourFor(core::AttackOutcome::Sunk);

        EXPECT_TRUE(behaviour.keepsTurn());
        EXPECT_TRUE(behaviour.isShotSpent());
        EXPECT_TRUE(behaviour.grantsSkill());
        EXPECT_EQ(behaviour.eventKind(), MatchEventKind::ShipSunk);
    }

    TEST(AttackOutcomeBehaviourTests, ARejectedShotCostsNothing) {
        for (
            const core::AttackOutcome outcome :
            {core::AttackOutcome::AlreadyAttacked, core::AttackOutcome::OutOfBounds}
        ) {
            const AttackOutcomeBehaviour& behaviour = behaviourFor(outcome);

            EXPECT_TRUE(behaviour.keepsTurn());
            EXPECT_FALSE(behaviour.isShotSpent());
            EXPECT_FALSE(behaviour.grantsSkill());
            EXPECT_EQ(behaviour.eventKind(), MatchEventKind::ShotRejected);
        }
    }

    TEST(AttackOutcomeBehaviourTests, EveryOutcomeHasABehaviour) {
        for (
            const core::AttackOutcome outcome :
            {core::AttackOutcome::Miss,
             core::AttackOutcome::Hit,
             core::AttackOutcome::Sunk,
             core::AttackOutcome::AlreadyAttacked,
             core::AttackOutcome::OutOfBounds}
        ) {
            EXPECT_NO_THROW((void)behaviourFor(outcome));
        }
    }

    TEST(AttackOutcomeBehaviourTests, TheSameOutcomeAlwaysGivesTheSameInstance) {
        EXPECT_EQ(&behaviourFor(core::AttackOutcome::Hit), &behaviourFor(core::AttackOutcome::Hit));
        EXPECT_EQ(
            &behaviourFor(core::AttackOutcome::AlreadyAttacked),
            &behaviourFor(core::AttackOutcome::OutOfBounds)
        );
    }

    TEST(AttackOutcomeBehaviourTests, AMissLeavesTheHuntAlone) {
        RandomEngine randomEngine{1U};
        AiOpponent opponent{randomEngine};
        const core::Board board{5, 5};

        behaviourFor(core::AttackOutcome::Miss).updateHunt(opponent, {2, 2}, board);

        EXPECT_TRUE(opponent.memory().currentTargetHits.empty());
    }

    TEST(AttackOutcomeBehaviourTests, AHitBecomesPartOfTheHunt) {
        RandomEngine randomEngine{1U};
        AiOpponent opponent{randomEngine};
        const core::Board board{5, 5};

        behaviourFor(core::AttackOutcome::Hit).updateHunt(opponent, {2, 2}, board);

        EXPECT_EQ(opponent.memory().currentTargetHits.size(), 1U);
    }

    TEST(AttackOutcomeBehaviourTests, SinkingEndsTheHuntAndRulesOutTheSurroundingWater) {
        RandomEngine randomEngine{1U};
        AiOpponent opponent{randomEngine};
        core::Board board{5, 5};
        board.place({2, 2}, core::Direction::Horizontal, 1, 1);

        behaviourFor(core::AttackOutcome::Sunk).updateHunt(opponent, {2, 2}, board);

        const AiMemory memory = opponent.memory();
        EXPECT_TRUE(memory.currentTargetHits.empty());
        EXPECT_TRUE(memory.attemptedCoordinates.contains({1, 1}));
        EXPECT_TRUE(memory.attemptedCoordinates.contains({3, 3}));
    }
}  // namespace cpp_warships::flow
