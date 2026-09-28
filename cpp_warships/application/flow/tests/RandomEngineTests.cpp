#include <application/flow/RandomEngine.h>
#include <gtest/gtest.h>

namespace cpp_warships::flow {
    TEST(RandomEngineTests, TheSameSeedGivesTheSameSequence) {
        RandomEngine firstEngine{99U};
        RandomEngine secondEngine{99U};

        for (int draw = 0; draw < 10; ++draw) {
            EXPECT_EQ(firstEngine(), secondEngine());
        }
    }

    TEST(RandomEngineTests, DifferentSeedsDivergeQuickly) {
        RandomEngine firstEngine{1U};
        RandomEngine secondEngine{2U};

        EXPECT_NE(firstEngine(), secondEngine());
    }

    TEST(RandomEngineTests, ARandomlySeededEngineProduces) {
        RandomEngine engine = makeRandomlySeededEngine();

        EXPECT_NO_THROW((void)engine());
    }
}  // namespace cpp_warships::flow
