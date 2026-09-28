#include <application/flow/RandomEngine.h>
#include <application/flow/SkillKind.h>
#include <application/flow/SkillQueue.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <deque>
#include <optional>
#include <unordered_set>

namespace cpp_warships::flow {
    namespace {
        constexpr unsigned int FIXED_SEED = 20260928U;
    }  // namespace

    TEST(SkillQueueTests, StartsEmpty) {
        const SkillQueue queue;

        EXPECT_TRUE(queue.isEmpty());
        EXPECT_EQ(queue.next(), std::nullopt);
        EXPECT_TRUE(queue.pending().empty());
    }

    TEST(SkillQueueTests, RestoresBankedSkills) {
        const SkillQueue queue{std::deque<SkillKind>{SkillKind::Scanner, SkillKind::RandomStrike}};

        EXPECT_FALSE(queue.isEmpty());
        EXPECT_EQ(queue.pending().size(), 2U);
        EXPECT_EQ(queue.next(), SkillKind::Scanner);
    }

    TEST(SkillQueueTests, ConsumesOldestFirst) {
        SkillQueue queue;
        queue.grant(SkillKind::Scanner);
        queue.grant(SkillKind::DoubleDamage);

        EXPECT_EQ(queue.consume(), SkillKind::Scanner);
        EXPECT_EQ(queue.consume(), SkillKind::DoubleDamage);
        EXPECT_EQ(queue.consume(), std::nullopt);
        EXPECT_TRUE(queue.isEmpty());
    }

    TEST(SkillQueueTests, PeekingDoesNotConsume) {
        SkillQueue queue;
        queue.grant(SkillKind::RandomStrike);

        EXPECT_EQ(queue.next(), SkillKind::RandomStrike);
        EXPECT_EQ(queue.next(), SkillKind::RandomStrike);
        EXPECT_EQ(queue.pending().size(), 1U);
    }

    TEST(SkillQueueTests, GrantsOneOfEverySkillWhenShuffled) {
        RandomEngine randomEngine{FIXED_SEED};
        SkillQueue queue;

        queue.grantAllShuffled(randomEngine);

        EXPECT_EQ(queue.pending().size(), ALL_SKILL_KINDS.size());
        const std::unordered_set<SkillKind> banked{queue.pending().begin(), queue.pending().end()};
        EXPECT_EQ(banked.size(), ALL_SKILL_KINDS.size());
    }

    TEST(SkillQueueTests, GrantsOnlyKnownSkillsAtRandom) {
        RandomEngine randomEngine{FIXED_SEED};
        SkillQueue queue;

        for (int attempt = 0; attempt < 50; ++attempt) {
            const SkillKind granted = queue.grantRandom(randomEngine);

            EXPECT_NE(
                std::find(ALL_SKILL_KINDS.begin(), ALL_SKILL_KINDS.end(), granted),
                ALL_SKILL_KINDS.end()
            );
        }

        EXPECT_EQ(queue.pending().size(), 50U);
    }

    TEST(SkillQueueTests, GrantingAtRandomBanksWhatItReturns) {
        RandomEngine randomEngine{FIXED_SEED};
        SkillQueue queue;

        const SkillKind granted = queue.grantRandom(randomEngine);

        EXPECT_EQ(queue.next(), granted);
    }

    TEST(SkillQueueTests, TheSameSeedBanksTheSameSkills) {
        RandomEngine firstEngine{FIXED_SEED};
        RandomEngine secondEngine{FIXED_SEED};
        SkillQueue firstQueue;
        SkillQueue secondQueue;

        firstQueue.grantAllShuffled(firstEngine);
        secondQueue.grantAllShuffled(secondEngine);

        EXPECT_EQ(firstQueue.pending(), secondQueue.pending());
    }

    TEST(SkillKindTests, ListsEverySkillExactlyOnce) {
        const std::unordered_set<SkillKind> distinct{
            ALL_SKILL_KINDS.begin(),
            ALL_SKILL_KINDS.end()
        };

        EXPECT_EQ(distinct.size(), ALL_SKILL_KINDS.size());
        EXPECT_EQ(ALL_SKILL_KINDS.size(), 3U);
    }
}  // namespace cpp_warships::flow
