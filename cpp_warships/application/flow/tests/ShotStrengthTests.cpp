#include <application/flow/ShotStrength.h>
#include <gtest/gtest.h>

namespace cpp_warships::flow {
    TEST(ShotStrengthTests, StartsUnarmedAtTheBaseDamage) {
        const ShotStrength strength{3};

        EXPECT_EQ(strength.baseDamage(), 3);
        EXPECT_EQ(strength.nextShotDamage(), 3);
        EXPECT_FALSE(strength.isDoubleDamageArmed());
    }

    TEST(ShotStrengthTests, RestoresAnArmedBonus) {
        const ShotStrength strength{3, true};

        EXPECT_TRUE(strength.isDoubleDamageArmed());
        EXPECT_EQ(strength.nextShotDamage(), 6);
    }

    TEST(ShotStrengthTests, ArmingDoublesTheNextShot) {
        ShotStrength strength{2};

        strength.armDoubleDamage();

        EXPECT_TRUE(strength.isDoubleDamageArmed());
        EXPECT_EQ(strength.nextShotDamage(), 4);
        EXPECT_EQ(strength.baseDamage(), 2);
    }

    TEST(ShotStrengthTests, SpendingReturnsToTheBaseDamage) {
        ShotStrength strength{2};
        strength.armDoubleDamage();

        strength.spend();

        EXPECT_FALSE(strength.isDoubleDamageArmed());
        EXPECT_EQ(strength.nextShotDamage(), 2);
    }

    TEST(ShotStrengthTests, SpendingWhenUnarmedChangesNothing) {
        ShotStrength strength{2};

        strength.spend();

        EXPECT_EQ(strength.nextShotDamage(), 2);
    }

    TEST(ShotStrengthTests, ArmingTwiceStillOnlyDoubles) {
        ShotStrength strength{2};

        strength.armDoubleDamage();
        strength.armDoubleDamage();

        EXPECT_EQ(strength.nextShotDamage(), 4);
    }
}  // namespace cpp_warships::flow
