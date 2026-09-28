#include <application/core/Board.h>
#include <application/core/Coordinate.h>
#include <application/flow/MatchEvent.h>
#include <application/flow/RandomEngine.h>
#include <application/flow/SkillBehaviour.h>
#include <application/flow/SkillKind.h>
#include <application/flow/SkillManager.h>
#include <application/flow/SkillQueue.h>
#include <gtest/gtest.h>

#include <deque>
#include <optional>
#include <vector>

namespace cpp_warships::flow {
    namespace {
        /** @brief A SkillContext that records what was asked of it instead of playing a match. */
        class StubSkillContext : public SkillContext {
        public:
            StubSkillContext(core::Board& board, RandomEngine& randomEngine)
                : board_(board)
                , randomEngine_(randomEngine) {}

            [[nodiscard]] const core::Board& enemyBoard() const override {
                return board_;
            }

            [[nodiscard]] RandomEngine& randomEngine() override {
                return randomEngine_;
            }

            void strikeEnemyCell(core::Coordinate) override {
                ++strikeCount;
            }

            void armDoubleDamage() override {
                ++armedCount;
            }

            void recordSkillEvent(const MatchEvent& event) override {
                recordedEvents.push_back(event);
            }

            std::vector<MatchEvent> recordedEvents;
            int strikeCount = 0;
            int armedCount = 0;

        private:
            core::Board& board_;
            RandomEngine& randomEngine_;
        };
    }  // namespace

    TEST(SkillManagerTests, StartsWithNothingBanked) {
        RandomEngine randomEngine{5U};
        const SkillManager manager{randomEngine};

        EXPECT_TRUE(manager.bank().isEmpty());
        EXPECT_FALSE(manager.nextNeedsTarget());
    }

    TEST(SkillManagerTests, RestoresABankOfSkills) {
        RandomEngine randomEngine{5U};
        const SkillManager manager{
            randomEngine,
            SkillQueue{std::deque<SkillKind>{SkillKind::DoubleDamage}}
        };

        EXPECT_EQ(manager.bank().next(), SkillKind::DoubleDamage);
    }

    TEST(SkillManagerTests, TheOpeningHandHoldsOneOfEverySkill) {
        RandomEngine randomEngine{5U};
        SkillManager manager{randomEngine};

        manager.grantOpeningHand();

        EXPECT_EQ(manager.bank().pending().size(), ALL_SKILL_KINDS.size());
    }

    TEST(SkillManagerTests, GrantingAtRandomBanksOneMore) {
        RandomEngine randomEngine{5U};
        SkillManager manager{randomEngine};

        const SkillKind granted = manager.grantRandom();

        EXPECT_EQ(manager.bank().pending().size(), 1U);
        EXPECT_EQ(manager.bank().next(), granted);
    }

    TEST(SkillManagerTests, OnlyTheScannerWaitingAtTheFrontNeedsATarget) {
        RandomEngine randomEngine{5U};
        const SkillManager scannerFirst{
            randomEngine,
            SkillQueue{std::deque<SkillKind>{SkillKind::Scanner, SkillKind::DoubleDamage}}
        };
        const SkillManager damageFirst{
            randomEngine,
            SkillQueue{std::deque<SkillKind>{SkillKind::DoubleDamage, SkillKind::Scanner}}
        };

        EXPECT_TRUE(scannerFirst.nextNeedsTarget());
        EXPECT_FALSE(damageFirst.nextNeedsTarget());
    }

    TEST(SkillManagerTests, ApplyingWithNothingBankedFails) {
        RandomEngine randomEngine{5U};
        core::Board board{6, 6};
        SkillManager manager{randomEngine};
        StubSkillContext context{board, randomEngine};

        EXPECT_FALSE(manager.applyNext(context, std::nullopt));
    }

    TEST(SkillManagerTests, ApplyingConsumesTheSkill) {
        RandomEngine randomEngine{5U};
        core::Board board{6, 6};
        SkillManager manager{
            randomEngine,
            SkillQueue{std::deque<SkillKind>{SkillKind::DoubleDamage}}
        };
        StubSkillContext context{board, randomEngine};

        EXPECT_TRUE(manager.applyNext(context, std::nullopt));

        EXPECT_TRUE(manager.bank().isEmpty());
        EXPECT_EQ(context.armedCount, 1);
    }

    TEST(SkillManagerTests, AScannerWithoutATargetIsRefusedAndStaysBanked) {
        RandomEngine randomEngine{5U};
        core::Board board{6, 6};
        SkillManager manager{randomEngine, SkillQueue{std::deque<SkillKind>{SkillKind::Scanner}}};
        StubSkillContext context{board, randomEngine};

        EXPECT_FALSE(manager.applyNext(context, std::nullopt));

        EXPECT_EQ(manager.bank().next(), SkillKind::Scanner);
        EXPECT_TRUE(context.recordedEvents.empty());
    }

    TEST(SkillManagerTests, AScannerWithATargetIsApplied) {
        RandomEngine randomEngine{5U};
        core::Board board{6, 6};
        SkillManager manager{randomEngine, SkillQueue{std::deque<SkillKind>{SkillKind::Scanner}}};
        StubSkillContext context{board, randomEngine};

        EXPECT_TRUE(manager.applyNext(context, core::Coordinate{2, 2}));

        EXPECT_TRUE(manager.bank().isEmpty());
        ASSERT_EQ(context.recordedEvents.size(), 1U);
        EXPECT_EQ(context.recordedEvents.front().kind, MatchEventKind::AreaScanned);
    }

    TEST(SkillManagerTests, ATargetIsIgnoredBySkillsThatDoNotNeedOne) {
        RandomEngine randomEngine{5U};
        core::Board board{6, 6};
        SkillManager manager{
            randomEngine,
            SkillQueue{std::deque<SkillKind>{SkillKind::DoubleDamage}}
        };
        StubSkillContext context{board, randomEngine};

        EXPECT_TRUE(manager.applyNext(context, core::Coordinate{1, 1}));

        EXPECT_EQ(context.armedCount, 1);
    }

    TEST(SkillManagerTests, SkillsAreAppliedInTheOrderTheyWereBanked) {
        RandomEngine randomEngine{5U};
        core::Board board{6, 6};
        SkillManager manager{
            randomEngine,
            SkillQueue{std::deque<SkillKind>{SkillKind::DoubleDamage, SkillKind::RandomStrike}}
        };
        StubSkillContext context{board, randomEngine};

        EXPECT_TRUE(manager.applyNext(context, std::nullopt));
        EXPECT_EQ(context.armedCount, 1);
        EXPECT_EQ(context.strikeCount, 0);

        EXPECT_TRUE(manager.applyNext(context, std::nullopt));
        EXPECT_EQ(context.strikeCount, 1);
    }
}  // namespace cpp_warships::flow
