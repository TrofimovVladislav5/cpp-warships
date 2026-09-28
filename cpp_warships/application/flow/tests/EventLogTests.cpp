#include <application/flow/EventLog.h>
#include <application/flow/MatchEvent.h>
#include <gtest/gtest.h>

namespace cpp_warships::flow {
    TEST(EventLogTests, StartsEmpty) {
        const EventLog log;

        EXPECT_TRUE(log.isEmpty());
        EXPECT_TRUE(log.entries().empty());
    }

    TEST(EventLogTests, KeepsEventsInTheOrderTheyHappened) {
        EventLog log;
        log.record({.kind = MatchEventKind::ShotMissed});
        log.record({.kind = MatchEventKind::ShipSunk});

        ASSERT_EQ(log.entries().size(), 2U);
        EXPECT_EQ(log.entries()[0].kind, MatchEventKind::ShotMissed);
        EXPECT_EQ(log.entries()[1].kind, MatchEventKind::ShipSunk);
        EXPECT_FALSE(log.isEmpty());
    }

    TEST(EventLogTests, DrainingHandsOverEverythingRecorded) {
        EventLog log;
        log.record({.kind = MatchEventKind::ShipDamaged, .actor = Participant::Computer});

        const MatchEventLog drained = log.drain();

        ASSERT_EQ(drained.size(), 1U);
        EXPECT_EQ(drained.front().kind, MatchEventKind::ShipDamaged);
        EXPECT_EQ(drained.front().actor, Participant::Computer);
    }

    TEST(EventLogTests, DrainingStartsAFreshPage) {
        EventLog log;
        log.record({.kind = MatchEventKind::ShotMissed});

        log.drain();

        EXPECT_TRUE(log.isEmpty());
        EXPECT_TRUE(log.drain().empty());
    }

    TEST(EventLogTests, CarriesEveryDetailOfAnEvent) {
        EventLog log;
        log.record(
            {.kind = MatchEventKind::AreaScanned,
             .actor = Participant::Player,
             .coordinate = core::Coordinate{4, 5},
             .skill = SkillKind::Scanner,
             .scanFoundShip = true}
        );

        const MatchEvent& event = log.entries().front();

        EXPECT_EQ(event.coordinate, (core::Coordinate{4, 5}));
        EXPECT_EQ(event.skill, SkillKind::Scanner);
        EXPECT_TRUE(event.scanFoundShip);
    }

    TEST(EventLogTests, AnEventDefaultsToThePlayerWithNothingAttached) {
        const MatchEvent event{.kind = MatchEventKind::TurnPassed};

        EXPECT_EQ(event.actor, Participant::Player);
        EXPECT_FALSE(event.coordinate.has_value());
        EXPECT_FALSE(event.skill.has_value());
        EXPECT_FALSE(event.scanFoundShip);
    }
}  // namespace cpp_warships::flow
