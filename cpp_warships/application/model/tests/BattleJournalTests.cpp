#include <application/flow/MatchEvent.h>
#include <application/model/BattleJournal.h>
#include <gtest/gtest.h>

namespace cpp_warships::model {
    namespace {
        /** @brief A log of @p count plain turn-passed events. */
        flow::MatchEventLog eventsOf(int count) {
            flow::MatchEventLog events;
            for (int index = 0; index < count; ++index) {
                events.push_back({.kind = flow::MatchEventKind::TurnPassed});
            }

            return events;
        }
    }  // namespace

    TEST(BattleJournalTests, StartsEmpty) {
        const BattleJournal journal;

        EXPECT_TRUE(journal.isEmpty());
        EXPECT_TRUE(journal.entries().empty());
    }

    TEST(BattleJournalTests, KeepsWhatItAbsorbsOldestFirst) {
        BattleJournal journal;
        flow::MatchEventLog events;
        events.push_back({.kind = flow::MatchEventKind::ShotMissed});
        events.push_back({.kind = flow::MatchEventKind::ShipSunk});

        journal.absorb(events);

        ASSERT_EQ(journal.entries().size(), 2U);
        EXPECT_EQ(journal.entries().front().kind, flow::MatchEventKind::ShotMissed);
        EXPECT_EQ(journal.entries().back().kind, flow::MatchEventKind::ShipSunk);
        EXPECT_FALSE(journal.isEmpty());
    }

    TEST(BattleJournalTests, AbsorbingAgainAppends) {
        BattleJournal journal;
        journal.absorb(eventsOf(2));

        journal.absorb(eventsOf(3));

        EXPECT_EQ(journal.entries().size(), 5U);
    }

    TEST(BattleJournalTests, AbsorbingNothingChangesNothing) {
        BattleJournal journal;
        journal.absorb(eventsOf(2));

        journal.absorb({});

        EXPECT_EQ(journal.entries().size(), 2U);
    }

    TEST(BattleJournalTests, TrimsToWhatAPanelCanShow) {
        BattleJournal journal;

        journal.absorb(eventsOf(250));

        EXPECT_EQ(journal.entries().size(), 200U);
    }

    TEST(BattleJournalTests, TrimmingDropsTheOldestFirst) {
        BattleJournal journal;
        journal.absorb(eventsOf(200));
        flow::MatchEventLog newest;
        newest.push_back({.kind = flow::MatchEventKind::MatchLost});

        journal.absorb(newest);

        EXPECT_EQ(journal.entries().size(), 200U);
        EXPECT_EQ(journal.entries().back().kind, flow::MatchEventKind::MatchLost);
    }

    TEST(BattleJournalTests, ClearingEmptiesIt) {
        BattleJournal journal;
        journal.absorb(eventsOf(3));

        journal.clear();

        EXPECT_TRUE(journal.isEmpty());
    }

    TEST(BattleJournalTests, CarriesEveryDetailOfAnEvent) {
        BattleJournal journal;
        flow::MatchEventLog events;
        events.push_back(
            {.kind = flow::MatchEventKind::AreaScanned,
             .actor = flow::Participant::Player,
             .coordinate = core::Coordinate{2, 3},
             .skill = flow::SkillKind::Scanner,
             .scanFoundShip = true}
        );

        journal.absorb(events);

        const flow::MatchEvent& kept = journal.entries().front();
        EXPECT_EQ(kept.coordinate, (core::Coordinate{2, 3}));
        EXPECT_EQ(kept.skill, flow::SkillKind::Scanner);
        EXPECT_TRUE(kept.scanFoundShip);
    }
}  // namespace cpp_warships::model
