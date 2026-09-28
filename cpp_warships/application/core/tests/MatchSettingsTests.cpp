#include <application/core/FleetComposition.h>
#include <application/core/MatchSettings.h>
#include <application/core/Segment.h>
#include <gtest/gtest.h>

#include <map>

namespace cpp_warships::core {
    TEST(MatchSettingsTests, KeepsWhatItWasGiven) {
        const MatchSettings settings{12, FleetComposition{std::map<int, int>{{2, 1}}}, 3, 4};

        EXPECT_EQ(settings.boardSize(), 12);
        EXPECT_EQ(settings.baseDamage(), 3);
        EXPECT_EQ(settings.segmentHealth(), 4);
        EXPECT_EQ(settings.fleet().countOf(2), 1);
    }

    TEST(MatchSettingsTests, DefaultsToOneDamageAndTheDefaultSegmentHealth) {
        const MatchSettings settings{10, FleetComposition{}};

        EXPECT_EQ(settings.baseDamage(), 1);
        EXPECT_EQ(settings.segmentHealth(), DEFAULT_SEGMENT_HEALTH);
    }

    TEST(MatchSettingsTests, ScalesTheFleetToTheBoardSize) {
        const MatchSettings settings = MatchSettings::forBoardSize(10);

        EXPECT_EQ(settings.boardSize(), 10);
        EXPECT_EQ(
            settings.fleet().countsByLength(),
            FleetComposition::forBoardSize(10).countsByLength()
        );
    }

    TEST(MatchSettingsTests, ScaledSettingsUseTheDefaultCombatValues) {
        const MatchSettings settings = MatchSettings::forBoardSize(15);

        EXPECT_EQ(settings.baseDamage(), 1);
        EXPECT_EQ(settings.segmentHealth(), DEFAULT_SEGMENT_HEALTH);
    }
}  // namespace cpp_warships::core
