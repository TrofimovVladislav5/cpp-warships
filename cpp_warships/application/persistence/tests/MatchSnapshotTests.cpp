#include <application/core/Board.h>
#include <application/core/Direction.h>
#include <application/core/MatchSettings.h>
#include <application/flow/AiOpponent.h>
#include <application/flow/Match.h>
#include <application/flow/MatchEvent.h>
#include <application/flow/MatchPhase.h>
#include <application/flow/Participant.h>
#include <application/flow/RandomEngine.h>
#include <application/flow/SkillKind.h>
#include <application/persistence/MatchSnapshot.h>
#include <application/persistence/serializers/MatchSnapshotJsonSerializer.h>
#include <gtest/gtest.h>
#include <serialization/exceptions/DeserializationException.h>

#include <cstddef>
#include <deque>
#include <nlohmann/json.hpp>
#include <utility>

namespace cpp_warships::persistence {
    namespace {
        constexpr unsigned int FIXED_SEED = 2024U;
        constexpr int BOARD_SIZE = 8;

        /** @brief A snapshot serializer with the whole chain below it wired up. */
        class WiredSnapshotSerializer {
        public:
            WiredSnapshotSerializer() {
                shipSerializer.setChildrenSerializers(&segmentSerializer);
                boardSerializer.setChildrenSerializers(&shipSerializer);
                serializer.setChildrenSerializers(&boardSerializer, &settingsSerializer);
            }

            serializers::MatchSnapshotJsonSerializer serializer;

        private:
            serializers::SegmentJsonSerializer segmentSerializer;
            serializers::ShipJsonSerializer shipSerializer;
            serializers::BoardJsonSerializer boardSerializer;
            serializers::MatchSettingsJsonSerializer settingsSerializer;
        };

        MatchSnapshot makeSnapshot() {
            core::Board playerBoard{BOARD_SIZE, BOARD_SIZE};
            playerBoard.place({0, 0}, core::Direction::Horizontal, 2, 2);
            playerBoard.attack({0, 0}, 1);

            core::Board computerBoard{BOARD_SIZE, BOARD_SIZE};
            computerBoard.place({5, 5}, core::Direction::Vertical, 3, 2);

            flow::AiMemory memory;
            memory.attemptedCoordinates = {{1, 1}, {2, 2}};
            memory.currentTargetHits = {{3, 3}};

            flow::MatchEventLog journal;
            journal.push_back(
                {.kind = flow::MatchEventKind::ShipDamaged,
                 .actor = flow::Participant::Player,
                 .coordinate = core::Coordinate{4, 4}}
            );
            journal.push_back(
                {.kind = flow::MatchEventKind::AreaScanned,
                 .actor = flow::Participant::Player,
                 .coordinate = core::Coordinate{2, 2},
                 .skill = flow::SkillKind::Scanner,
                 .scanFoundShip = true}
            );

            return MatchSnapshot{
                core::MatchSettings::forBoardSize(BOARD_SIZE),
                std::move(playerBoard),
                std::move(computerBoard),
                std::deque<flow::SkillKind>{
                    flow::SkillKind::Scanner,
                    flow::SkillKind::DoubleDamage
                },
                3,
                flow::MatchPhase::Battle,
                flow::Participant::Computer,
                true,
                memory,
                journal
            };
        }
    }  // namespace

    TEST(MatchSnapshotTests, KeepsEverythingItWasGiven) {
        const MatchSnapshot snapshot = makeSnapshot();

        EXPECT_EQ(snapshot.settings().boardSize(), BOARD_SIZE);
        EXPECT_EQ(snapshot.roundNumber(), 3);
        EXPECT_EQ(snapshot.phase(), flow::MatchPhase::Battle);
        EXPECT_EQ(snapshot.currentTurn(), flow::Participant::Computer);
        EXPECT_TRUE(snapshot.isDoubleDamageArmed());
        EXPECT_EQ(snapshot.bankedSkills().size(), 2U);
        EXPECT_EQ(snapshot.journal().size(), 2U);
        EXPECT_TRUE(snapshot.playerBoard().hasShips());
        EXPECT_TRUE(snapshot.opponentMemory().attemptedCoordinates.contains({1, 1}));
    }

    TEST(MatchSnapshotTests, NamesItsType) {
        MatchSnapshot snapshot = makeSnapshot();

        EXPECT_EQ(snapshot.getType(), "MatchSnapshot");
    }

    TEST(MatchSnapshotTests, CapturesAMatchAsItStands) {
        flow::RandomEngine randomEngine{FIXED_SEED};
        flow::Match match{core::MatchSettings::forBoardSize(BOARD_SIZE), randomEngine};
        ASSERT_TRUE(match.shufflePlayerFleet());
        ASSERT_TRUE(match.beginBattle());
        const flow::MatchEventLog journal = match.drainEvents();

        const MatchSnapshot snapshot = MatchSnapshot::capture(match, journal);

        EXPECT_EQ(snapshot.phase(), match.phase());
        EXPECT_EQ(snapshot.roundNumber(), match.roundNumber());
        EXPECT_EQ(snapshot.currentTurn(), match.currentTurn());
        EXPECT_EQ(snapshot.bankedSkills(), match.skills().pending());
        EXPECT_EQ(snapshot.playerBoard().ships().size(), match.playerBoard().ships().size());
    }

    TEST(MatchSnapshotTests, CarriesAJournalTheMatchHasForgotten) {
        flow::RandomEngine randomEngine{FIXED_SEED};
        const flow::Match match{core::MatchSettings::forBoardSize(BOARD_SIZE), randomEngine};
        flow::MatchEventLog journal;
        journal.push_back({.kind = flow::MatchEventKind::RoundWon});

        const MatchSnapshot snapshot = MatchSnapshot::capture(match, journal);

        ASSERT_EQ(snapshot.journal().size(), 1U);
        EXPECT_EQ(snapshot.journal().front().kind, flow::MatchEventKind::RoundWon);
    }

    TEST(MatchSnapshotTests, RestoresAMatchThatPlaysOn) {
        flow::RandomEngine randomEngine{FIXED_SEED};
        const MatchSnapshot snapshot = makeSnapshot();

        const flow::Match restored = snapshot.restore(randomEngine);

        EXPECT_EQ(restored.roundNumber(), 3);
        EXPECT_EQ(restored.phase(), flow::MatchPhase::Battle);
        EXPECT_EQ(restored.currentTurn(), flow::Participant::Computer);
        EXPECT_TRUE(restored.isDoubleDamageArmed());
        EXPECT_EQ(restored.skills().pending(), snapshot.bankedSkills());
        EXPECT_EQ(
            restored.opponentMemory().attemptedCoordinates,
            snapshot.opponentMemory().attemptedCoordinates
        );
    }

    TEST(MatchSnapshotTests, AMatchSurvivesCaptureAndRestore) {
        flow::RandomEngine randomEngine{FIXED_SEED};
        flow::Match original{core::MatchSettings::forBoardSize(BOARD_SIZE), randomEngine};
        ASSERT_TRUE(original.shufflePlayerFleet());
        ASSERT_TRUE(original.beginBattle());

        const flow::Match restored =
            MatchSnapshot::capture(original, original.drainEvents()).restore(randomEngine);

        EXPECT_EQ(restored.playerBoard().ships().size(), original.playerBoard().ships().size());
        EXPECT_EQ(restored.computerBoard().ships().size(), original.computerBoard().ships().size());
        EXPECT_EQ(restored.phase(), original.phase());
    }

    TEST(MatchSnapshotJsonSerializerTests, NamesItsType) {
        WiredSnapshotSerializer wired;

        EXPECT_EQ(wired.serializer.getType(), "MatchSnapshot");
    }

    TEST(MatchSnapshotJsonSerializerTests, ReadsBackEveryPartOfAMatch) {
        WiredSnapshotSerializer wired;
        MatchSnapshot original = makeSnapshot();

        const MatchSnapshot restored =
            wired.serializer.deserialize(wired.serializer.serialize(original));

        EXPECT_EQ(restored.roundNumber(), original.roundNumber());
        EXPECT_EQ(restored.phase(), original.phase());
        EXPECT_EQ(restored.currentTurn(), original.currentTurn());
        EXPECT_EQ(restored.isDoubleDamageArmed(), original.isDoubleDamageArmed());
        EXPECT_EQ(restored.bankedSkills(), original.bankedSkills());
        EXPECT_EQ(restored.settings().boardSize(), original.settings().boardSize());
    }

    TEST(MatchSnapshotJsonSerializerTests, CarriesBothBoardsBack) {
        WiredSnapshotSerializer wired;
        MatchSnapshot original = makeSnapshot();

        const MatchSnapshot restored =
            wired.serializer.deserialize(wired.serializer.serialize(original));

        EXPECT_EQ(restored.playerBoard().attackedCells(), original.playerBoard().attackedCells());
        EXPECT_EQ(restored.computerBoard().ships().size(), original.computerBoard().ships().size());
    }

    TEST(MatchSnapshotJsonSerializerTests, CarriesTheOpponentsMemoryBack) {
        WiredSnapshotSerializer wired;
        MatchSnapshot original = makeSnapshot();

        const MatchSnapshot restored =
            wired.serializer.deserialize(wired.serializer.serialize(original));

        EXPECT_EQ(
            restored.opponentMemory().attemptedCoordinates,
            original.opponentMemory().attemptedCoordinates
        );
        EXPECT_EQ(
            restored.opponentMemory().currentTargetHits,
            original.opponentMemory().currentTargetHits
        );
    }

    TEST(MatchSnapshotJsonSerializerTests, CarriesTheJournalBackInFull) {
        WiredSnapshotSerializer wired;
        MatchSnapshot original = makeSnapshot();

        const MatchSnapshot restored =
            wired.serializer.deserialize(wired.serializer.serialize(original));

        ASSERT_EQ(restored.journal().size(), original.journal().size());
        for (std::size_t index = 0; index < restored.journal().size(); ++index) {
            const flow::MatchEvent& before = original.journal()[index];
            const flow::MatchEvent& after = restored.journal()[index];

            EXPECT_EQ(after.kind, before.kind);
            EXPECT_EQ(after.actor, before.actor);
            EXPECT_EQ(after.coordinate, before.coordinate);
            EXPECT_EQ(after.skill, before.skill);
            EXPECT_EQ(after.scanFoundShip, before.scanFoundShip);
        }
    }

    TEST(MatchSnapshotJsonSerializerTests, RefusesJsonThatIsNotASnapshot) {
        WiredSnapshotSerializer wired;

        EXPECT_FALSE(wired.serializer.isRelated(nlohmann::json{{"roundNumber", 1}}));
        EXPECT_THROW(
            (void)wired.serializer.deserialize(nlohmann::json{{"roundNumber", 1}}),
            serialization::exceptions::DeserializationException
        );
    }
}  // namespace cpp_warships::persistence
