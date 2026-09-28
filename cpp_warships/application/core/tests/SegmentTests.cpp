#include <application/core/Segment.h>
#include <gtest/gtest.h>

namespace cpp_warships::core {
    TEST(SegmentTests, StartsAtFullHealth) {
        const Segment segment{3};

        EXPECT_EQ(segment.maximumHealth(), 3);
        EXPECT_EQ(segment.health(), 3);
        EXPECT_FALSE(segment.isDestroyed());
    }

    TEST(SegmentTests, RestoresAPartiallyDamagedSegment) {
        const Segment segment{3, 1};

        EXPECT_EQ(segment.maximumHealth(), 3);
        EXPECT_EQ(segment.health(), 1);
    }

    TEST(SegmentTests, ClampsRestoredHealthIntoRange) {
        EXPECT_EQ(Segment(3, 9).health(), 3);
        EXPECT_EQ(Segment(3, -4).health(), 0);
        EXPECT_TRUE(Segment(3, -4).isDestroyed());
    }

    TEST(SegmentTests, TakesDamageDownToZeroAndNoFurther) {
        Segment segment{DEFAULT_SEGMENT_HEALTH};

        segment.takeDamage(1);
        EXPECT_EQ(segment.health(), DEFAULT_SEGMENT_HEALTH - 1);
        EXPECT_FALSE(segment.isDestroyed());

        segment.takeDamage(99);
        EXPECT_EQ(segment.health(), 0);
        EXPECT_TRUE(segment.isDestroyed());
    }

    TEST(SegmentTests, KeepsMaximumHealthWhileTakingDamage) {
        Segment segment{5};
        segment.takeDamage(4);

        EXPECT_EQ(segment.health(), 1);
        EXPECT_EQ(segment.maximumHealth(), 5);
    }

    TEST(SegmentTests, DefaultSegmentHealthIsTwo) {
        EXPECT_EQ(DEFAULT_SEGMENT_HEALTH, 2);
    }
}  // namespace cpp_warships::core
