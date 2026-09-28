#include <application/core/Board.h>
#include <application/core/Coordinate.h>
#include <application/core/Direction.h>
#include <application/flow/MatchEvent.h>
#include <application/flow/RandomEngine.h>
#include <application/flow/SkillBehaviour.h>
#include <application/flow/SkillKind.h>
#include <gtest/gtest.h>

#include <optional>
#include <vector>

namespace cpp_warships::flow {
    namespace {
        /** @brief A SkillContext that records what was asked of it instead of playing a match. */
        class RecordingSkillContext : public SkillContext {
        public:
            RecordingSkillContext(core::Board& board, RandomEngine& randomEngine)
                : board_(board)
                , randomEngine_(randomEngine) {}

            [[nodiscard]] const core::Board& enemyBoard() const override {
                return board_;
            }

            [[nodiscard]] RandomEngine& randomEngine() override {
                return randomEngine_;
            }

            void strikeEnemyCell(core::Coordinate coordinate) override {
                struckCells.push_back(coordinate);
            }

            void armDoubleDamage() override {
                ++armedCount;
            }

            void recordSkillEvent(const MatchEvent& event) override {
                recordedEvents.push_back(event);
            }

            std::vector<core::Coordinate> struckCells;
            std::vector<MatchEvent> recordedEvents;
            int armedCount = 0;

        private:
            core::Board& board_;
            RandomEngine& randomEngine_;
        };
    }  // namespace

    TEST(SkillBehaviourTests, OnlyTheScannerNeedsATarget) {
        EXPECT_TRUE(behaviourFor(SkillKind::Scanner).needsTarget());
        EXPECT_FALSE(behaviourFor(SkillKind::DoubleDamage).needsTarget());
        EXPECT_FALSE(behaviourFor(SkillKind::RandomStrike).needsTarget());
    }

    TEST(SkillBehaviourTests, EverySkillHasABehaviour) {
        for (const SkillKind skill : ALL_SKILL_KINDS) {
            EXPECT_NO_THROW((void)behaviourFor(skill));
        }
    }

    TEST(SkillBehaviourTests, TheSameSkillAlwaysGivesTheSameInstance) {
        EXPECT_EQ(&behaviourFor(SkillKind::Scanner), &behaviourFor(SkillKind::Scanner));
    }

    TEST(SkillBehaviourTests, TheScannerReportsAShipItFinds) {
        core::Board board{8, 8};
        board.place({4, 4}, core::Direction::Horizontal, 1);
        RandomEngine randomEngine{7U};
        RecordingSkillContext context{board, randomEngine};

        behaviourFor(SkillKind::Scanner).apply(context, core::Coordinate{3, 3});

        ASSERT_EQ(context.recordedEvents.size(), 1U);
        const MatchEvent& event = context.recordedEvents.front();
        EXPECT_EQ(event.kind, MatchEventKind::AreaScanned);
        EXPECT_EQ(event.skill, SkillKind::Scanner);
        EXPECT_EQ(event.coordinate, (core::Coordinate{3, 3}));
        EXPECT_TRUE(event.scanFoundShip);
    }

    TEST(SkillBehaviourTests, TheScannerReportsEmptyWaterToo) {
        core::Board board{8, 8};
        board.place({0, 0}, core::Direction::Horizontal, 1);
        RandomEngine randomEngine{7U};
        RecordingSkillContext context{board, randomEngine};

        behaviourFor(SkillKind::Scanner).apply(context, core::Coordinate{6, 6});

        ASSERT_EQ(context.recordedEvents.size(), 1U);
        EXPECT_FALSE(context.recordedEvents.front().scanFoundShip);
    }

    TEST(SkillBehaviourTests, TheScannerFiresNoShot) {
        core::Board board{8, 8};
        RandomEngine randomEngine{7U};
        RecordingSkillContext context{board, randomEngine};

        behaviourFor(SkillKind::Scanner).apply(context, core::Coordinate{2, 2});

        EXPECT_TRUE(context.struckCells.empty());
        EXPECT_EQ(context.armedCount, 0);
    }

    TEST(SkillBehaviourTests, DoubleDamageArmsTheNextShotAndSaysSo) {
        core::Board board{8, 8};
        RandomEngine randomEngine{7U};
        RecordingSkillContext context{board, randomEngine};

        behaviourFor(SkillKind::DoubleDamage).apply(context, std::nullopt);

        EXPECT_EQ(context.armedCount, 1);
        ASSERT_EQ(context.recordedEvents.size(), 1U);
        EXPECT_EQ(context.recordedEvents.front().kind, MatchEventKind::DoubleDamageArmed);
        EXPECT_EQ(context.recordedEvents.front().skill, SkillKind::DoubleDamage);
        EXPECT_TRUE(context.struckCells.empty());
    }

    TEST(SkillBehaviourTests, ARandomStrikeFiresAtAnUnattackedCell) {
        core::Board board{6, 6};
        RandomEngine randomEngine{7U};
        RecordingSkillContext context{board, randomEngine};

        behaviourFor(SkillKind::RandomStrike).apply(context, std::nullopt);

        ASSERT_EQ(context.struckCells.size(), 1U);
        EXPECT_TRUE(board.contains(context.struckCells.front()));
        EXPECT_FALSE(board.attackedCells().contains(context.struckCells.front()));
    }

    TEST(SkillBehaviourTests, ARandomStrikeAddsNoEventOfItsOwn) {
        core::Board board{6, 6};
        RandomEngine randomEngine{7U};
        RecordingSkillContext context{board, randomEngine};

        behaviourFor(SkillKind::RandomStrike).apply(context, std::nullopt);

        EXPECT_TRUE(context.recordedEvents.empty());
    }

    TEST(SkillBehaviourTests, ARandomStrikeWithNowhereLeftToFireDoesNothing) {
        core::Board board{2, 2};
        for (int row = 0; row < 2; ++row) {
            for (int column = 0; column < 2; ++column) {
                board.attack({column, row}, 1);
            }
        }
        RandomEngine randomEngine{7U};
        RecordingSkillContext context{board, randomEngine};

        behaviourFor(SkillKind::RandomStrike).apply(context, std::nullopt);

        EXPECT_TRUE(context.struckCells.empty());
    }
}  // namespace cpp_warships::flow
