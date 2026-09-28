#include <application/core/Board.h>
#include <application/core/Coordinate.h>
#include <application/core/Direction.h>
#include <application/core/Outcomes.h>
#include <application/flow/AiOpponent.h>
#include <application/flow/RandomEngine.h>
#include <gtest/gtest.h>

#include <optional>
#include <unordered_set>
#include <vector>

namespace cpp_warships::flow {
    namespace {
        constexpr unsigned int FIXED_SEED = 31337U;
        constexpr int BOARD_SIZE = 8;

        /** @brief Marks every cell of @p board as already tried by @p opponent. */
        void exhaustBoard(AiOpponent& opponent, const core::Board& board) {
            for (int row = 0; row < board.height(); ++row) {
                for (int column = 0; column < board.width(); ++column) {
                    opponent.markAttempted({column, row});
                }
            }
        }
    }  // namespace

    TEST(AiOpponentTests, StartsWithNothingLearned) {
        RandomEngine randomEngine{FIXED_SEED};
        const AiOpponent opponent{randomEngine};

        const AiMemory memory = opponent.memory();
        EXPECT_TRUE(memory.attemptedCoordinates.empty());
        EXPECT_TRUE(memory.currentTargetHits.empty());
    }

    TEST(AiOpponentTests, RestoresWhatASaveRemembered) {
        RandomEngine randomEngine{FIXED_SEED};
        AiMemory restored;
        restored.attemptedCoordinates = {{1, 1}, {2, 2}};
        restored.currentTargetHits = {{3, 3}};

        const AiOpponent opponent{randomEngine, restored};

        const AiMemory memory = opponent.memory();
        EXPECT_EQ(memory.attemptedCoordinates.size(), 2U);
        EXPECT_TRUE(memory.attemptedCoordinates.contains({1, 1}));
        EXPECT_EQ(memory.currentTargetHits.size(), 1U);
    }

    TEST(AiOpponentTests, ChoosesACellItHasNotTried) {
        RandomEngine randomEngine{FIXED_SEED};
        AiOpponent opponent{randomEngine};
        const core::Board board{BOARD_SIZE, BOARD_SIZE};

        for (int shot = 0; shot < BOARD_SIZE * BOARD_SIZE; ++shot) {
            const std::optional<core::Coordinate> target = opponent.chooseTarget(board);

            ASSERT_TRUE(target.has_value());
            EXPECT_TRUE(board.contains(*target));
            EXPECT_FALSE(opponent.memory().attemptedCoordinates.contains(*target));
            opponent.markAttempted(*target);
        }
    }

    TEST(AiOpponentTests, GivesUpWhenEveryCellHasBeenTried) {
        RandomEngine randomEngine{FIXED_SEED};
        AiOpponent opponent{randomEngine};
        const core::Board board{BOARD_SIZE, BOARD_SIZE};
        exhaustBoard(opponent, board);

        EXPECT_EQ(opponent.chooseTarget(board), std::nullopt);
    }

    TEST(AiOpponentTests, RemembersACellItHasFiredAt) {
        RandomEngine randomEngine{FIXED_SEED};
        AiOpponent opponent{randomEngine};

        opponent.markAttempted({4, 4});

        EXPECT_TRUE(opponent.memory().attemptedCoordinates.contains({4, 4}));
    }

    TEST(AiOpponentTests, DoesNotChaseTheSameHitTwice) {
        RandomEngine randomEngine{FIXED_SEED};
        AiOpponent opponent{randomEngine};

        opponent.registerHit({2, 2});
        opponent.registerHit({2, 2});

        EXPECT_EQ(opponent.memory().currentTargetHits.size(), 1U);
    }

    TEST(AiOpponentTests, WorksOutwardsFromASingleHit) {
        RandomEngine randomEngine{FIXED_SEED};
        AiOpponent opponent{randomEngine};
        const core::Board board{BOARD_SIZE, BOARD_SIZE};
        opponent.markAttempted({4, 4});
        opponent.registerHit({4, 4});

        const std::optional<core::Coordinate> target = opponent.chooseTarget(board);

        ASSERT_TRUE(target.has_value());
        const std::unordered_set<core::Coordinate> neighbours{{3, 4}, {5, 4}, {4, 3}, {4, 5}};
        EXPECT_TRUE(neighbours.contains(*target));
    }

    TEST(AiOpponentTests, FinishesASegmentStillHoldingBeforeMovingOn) {
        RandomEngine randomEngine{FIXED_SEED};
        AiOpponent opponent{randomEngine};
        core::Board board{BOARD_SIZE, BOARD_SIZE};
        board.place({4, 4}, core::Direction::Horizontal, 2, 2);
        board.attack({4, 4}, 1);
        opponent.registerHit({4, 4});

        EXPECT_EQ(opponent.chooseTarget(board), (core::Coordinate{4, 4}));
    }

    TEST(AiOpponentTests, ExtendsAlongARunOfHits) {
        RandomEngine randomEngine{FIXED_SEED};
        AiOpponent opponent{randomEngine};
        const core::Board board{BOARD_SIZE, BOARD_SIZE};
        for (const core::Coordinate hit : {core::Coordinate{3, 4}, core::Coordinate{4, 4}}) {
            opponent.markAttempted(hit);
            opponent.registerHit(hit);
        }

        const std::optional<core::Coordinate> target = opponent.chooseTarget(board);

        ASSERT_TRUE(target.has_value());
        const std::unordered_set<core::Coordinate> ends{{2, 4}, {5, 4}};
        EXPECT_TRUE(ends.contains(*target));
    }

    TEST(AiOpponentTests, ExtendsAlongAVerticalRunToo) {
        RandomEngine randomEngine{FIXED_SEED};
        AiOpponent opponent{randomEngine};
        const core::Board board{BOARD_SIZE, BOARD_SIZE};
        for (const core::Coordinate hit : {core::Coordinate{4, 3}, core::Coordinate{4, 4}}) {
            opponent.markAttempted(hit);
            opponent.registerHit(hit);
        }

        const std::optional<core::Coordinate> target = opponent.chooseTarget(board);

        ASSERT_TRUE(target.has_value());
        const std::unordered_set<core::Coordinate> ends{{4, 2}, {4, 5}};
        EXPECT_TRUE(ends.contains(*target));
    }

    TEST(AiOpponentTests, FallsBackToRandomWhenBothEndsOfARunAreSpent) {
        RandomEngine randomEngine{FIXED_SEED};
        AiOpponent opponent{randomEngine};
        const core::Board board{BOARD_SIZE, BOARD_SIZE};
        for (const core::Coordinate hit : {core::Coordinate{3, 0}, core::Coordinate{4, 0}}) {
            opponent.markAttempted(hit);
            opponent.registerHit(hit);
        }
        opponent.markAttempted({2, 0});
        opponent.markAttempted({5, 0});

        const std::optional<core::Coordinate> target = opponent.chooseTarget(board);

        ASSERT_TRUE(target.has_value());
        const std::unordered_set<core::Coordinate> ends{{2, 0}, {5, 0}};
        EXPECT_FALSE(ends.contains(*target));
    }

    TEST(AiOpponentTests, FinishingAHuntRulesOutTheWaterAroundTheShip) {
        RandomEngine randomEngine{FIXED_SEED};
        AiOpponent opponent{randomEngine};
        const core::Board board{BOARD_SIZE, BOARD_SIZE};
        opponent.registerHit({4, 4});

        opponent.finishHunt(board);

        const AiMemory memory = opponent.memory();
        EXPECT_TRUE(memory.currentTargetHits.empty());
        for (
            const core::Coordinate ringCell :
            {core::Coordinate{3, 3}, core::Coordinate{4, 3}, core::Coordinate{5, 5}}
        ) {
            EXPECT_TRUE(memory.attemptedCoordinates.contains(ringCell));
        }
    }

    TEST(AiOpponentTests, FinishingAHuntStaysInsideTheBoard) {
        RandomEngine randomEngine{FIXED_SEED};
        AiOpponent opponent{randomEngine};
        const core::Board board{BOARD_SIZE, BOARD_SIZE};
        opponent.registerHit({0, 0});

        opponent.finishHunt(board);

        for (const core::Coordinate cell : opponent.memory().attemptedCoordinates) {
            EXPECT_TRUE(board.contains(cell));
        }
    }

    TEST(AiOpponentTests, AMissIsNotedAndChasesNothing) {
        RandomEngine randomEngine{FIXED_SEED};
        AiOpponent opponent{randomEngine};
        core::Board board{BOARD_SIZE, BOARD_SIZE};
        board.attack({1, 1}, 1);

        opponent.recordOutcome({1, 1}, core::AttackOutcome::Miss, board);

        const AiMemory memory = opponent.memory();
        EXPECT_TRUE(memory.attemptedCoordinates.contains({1, 1}));
        EXPECT_TRUE(memory.currentTargetHits.empty());
    }

    TEST(AiOpponentTests, ASegmentLeftHoldingIsNotWrittenOff) {
        RandomEngine randomEngine{FIXED_SEED};
        AiOpponent opponent{randomEngine};
        core::Board board{BOARD_SIZE, BOARD_SIZE};
        board.place({4, 4}, core::Direction::Horizontal, 2, 2);
        board.attack({4, 4}, 1);

        opponent.recordOutcome({4, 4}, core::AttackOutcome::Hit, board);

        const AiMemory memory = opponent.memory();
        EXPECT_FALSE(memory.attemptedCoordinates.contains({4, 4}));
        EXPECT_EQ(memory.currentTargetHits.size(), 1U);
    }

    TEST(AiOpponentTests, AFlattenedSegmentIsWrittenOffAndStillChased) {
        RandomEngine randomEngine{FIXED_SEED};
        AiOpponent opponent{randomEngine};
        core::Board board{BOARD_SIZE, BOARD_SIZE};
        board.place({4, 4}, core::Direction::Horizontal, 2, 1);
        board.attack({4, 4}, 1);

        opponent.recordOutcome({4, 4}, core::AttackOutcome::Hit, board);

        const AiMemory memory = opponent.memory();
        EXPECT_TRUE(memory.attemptedCoordinates.contains({4, 4}));
        EXPECT_EQ(memory.currentTargetHits.size(), 1U);
    }

    TEST(AiOpponentTests, SinkingClosesTheHunt) {
        RandomEngine randomEngine{FIXED_SEED};
        AiOpponent opponent{randomEngine};
        core::Board board{BOARD_SIZE, BOARD_SIZE};
        board.place({4, 4}, core::Direction::Horizontal, 1, 1);
        board.attack({4, 4}, 1);

        opponent.recordOutcome({4, 4}, core::AttackOutcome::Sunk, board);

        EXPECT_TRUE(opponent.memory().currentTargetHits.empty());
        EXPECT_TRUE(opponent.memory().attemptedCoordinates.contains({3, 3}));
    }

    TEST(AiOpponentTests, TheSameSeedChoosesTheSameCells) {
        RandomEngine firstEngine{FIXED_SEED};
        RandomEngine secondEngine{FIXED_SEED};
        AiOpponent firstOpponent{firstEngine};
        AiOpponent secondOpponent{secondEngine};
        const core::Board board{BOARD_SIZE, BOARD_SIZE};

        for (int shot = 0; shot < 20; ++shot) {
            const std::optional<core::Coordinate> first = firstOpponent.chooseTarget(board);
            const std::optional<core::Coordinate> second = secondOpponent.chooseTarget(board);

            ASSERT_EQ(first, second);
            firstOpponent.markAttempted(*first);
            secondOpponent.markAttempted(*second);
        }
    }

    TEST(AiOpponentTests, MemoryCarriesAcrossAReload) {
        RandomEngine randomEngine{FIXED_SEED};
        AiOpponent opponent{randomEngine};
        opponent.markAttempted({1, 2});
        opponent.registerHit({3, 4});

        const AiOpponent reloaded{randomEngine, opponent.memory()};

        EXPECT_EQ(reloaded.memory().attemptedCoordinates, opponent.memory().attemptedCoordinates);
        EXPECT_EQ(reloaded.memory().currentTargetHits, opponent.memory().currentTargetHits);
    }
}  // namespace cpp_warships::flow
