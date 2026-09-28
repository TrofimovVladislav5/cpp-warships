#include <application/flow/Participant.h>
#include <application/flow/TurnOrder.h>
#include <gtest/gtest.h>

namespace cpp_warships::flow {
    TEST(TurnOrderTests, NamesTheOtherSide) {
        EXPECT_EQ(opponentOf(Participant::Player), Participant::Computer);
        EXPECT_EQ(opponentOf(Participant::Computer), Participant::Player);
    }

    TEST(TurnOrderTests, StartsWithThePlayerByDefault) {
        const TurnOrder order;

        EXPECT_EQ(order.current(), Participant::Player);
        EXPECT_TRUE(order.isPlayerTurn());
    }

    TEST(TurnOrderTests, StartsWithWhoeverIsNamed) {
        const TurnOrder order{Participant::Computer};

        EXPECT_EQ(order.current(), Participant::Computer);
        EXPECT_FALSE(order.isPlayerTurn());
    }

    TEST(TurnOrderTests, PassingHandsTheTurnOver) {
        TurnOrder order{Participant::Player};

        order.pass();
        EXPECT_EQ(order.current(), Participant::Computer);

        order.pass();
        EXPECT_EQ(order.current(), Participant::Player);
    }

    TEST(TurnOrderTests, GivingNamesTheHolderOutright) {
        TurnOrder order{Participant::Player};

        order.giveTo(Participant::Computer);
        EXPECT_EQ(order.current(), Participant::Computer);

        order.giveTo(Participant::Computer);
        EXPECT_EQ(order.current(), Participant::Computer);
    }
}  // namespace cpp_warships::flow
