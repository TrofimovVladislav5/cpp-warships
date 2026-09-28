#include <application/core/Board.h>
#include <application/core/Coordinate.h>
#include <application/core/Direction.h>
#include <application/core/Ship.h>
#include <application/flow/Match.h>
#include <application/flow/MatchPhase.h>
#include <application/model/MatchInPlay.h>
#include <application/model/WarshipsGame.h>
#include <application/model/behaviors/MatchBehavior.h>
#include <application/model/behaviors/SaveBehavior.h>
#include <application/model/errors/ModelExceptions.h>
#include <application/persistence/MemorySaveStorage.h>
#include <application/persistence/SaveArchive.h>
#include <application/persistence/SaveSummary.h>
#include <gtest/gtest.h>

#include <cstddef>
#include <optional>
#include <string>
#include <vector>

namespace cpp_warships::model {
    namespace {
        constexpr int BOARD_SIZE = 10;

        /** @brief A whole game, with its storage and archive kept alive alongside. */
        class GameFixture {
        public:
            GameFixture()
                : archive_(storage_)
                , game_(randomEngine_, archive_) {}

            WarshipsGame& game() noexcept {
                return game_;
            }

            persistence::SaveArchive& archive() noexcept {
                return archive_;
            }

            persistence::MemorySaveStorage& storage() noexcept {
                return storage_;
            }

        private:
            flow::RandomEngine randomEngine_{8080U};
            persistence::MemorySaveStorage storage_;
            persistence::SaveArchive archive_;
            WarshipsGame game_;
        };

        /** @brief The first cell of the enemy fleet, once a battle is under way. */
        core::Coordinate firstEnemyCell(const WarshipsGame& game) {
            return game.match().computerBoard().ships().front().coordinateAt(0);
        }
    }  // namespace

    TEST(MatchInPlayTests, StartsWithNoMatch) {
        flow::RandomEngine randomEngine{1U};
        const MatchInPlay inPlay{randomEngine};

        EXPECT_FALSE(inPlay.hasMatch());
        EXPECT_TRUE(inPlay.journal().isEmpty());
        EXPECT_FALSE(inPlay.loadedFrom().has_value());
    }

    TEST(MatchInPlayTests, ReachingForAMatchThatIsNotThereIsRefused) {
        flow::RandomEngine randomEngine{1U};
        MatchInPlay inPlay{randomEngine};

        EXPECT_THROW((void)inPlay.match(), errors::NoMatchInPlayException);
        EXPECT_THROW((void)inPlay.editableMatch(), errors::NoMatchInPlayException);
    }

    TEST(MatchInPlayTests, PutsAMatchInPlay) {
        flow::RandomEngine randomEngine{1U};
        MatchInPlay inPlay{randomEngine};

        inPlay.replaceWith(
            flow::Match{core::MatchSettings::forBoardSize(BOARD_SIZE), randomEngine}
        );

        EXPECT_TRUE(inPlay.hasMatch());
        EXPECT_EQ(inPlay.match().phase(), flow::MatchPhase::Placement);
    }

    TEST(MatchInPlayTests, AFreshMatchCarriesNoHistoryAndNoSave) {
        flow::RandomEngine randomEngine{1U};
        MatchInPlay inPlay{randomEngine};
        inPlay.replaceWith(
            flow::Match{core::MatchSettings::forBoardSize(BOARD_SIZE), randomEngine},
            flow::MatchEventLog{{.kind = flow::MatchEventKind::RoundWon}},
            "a-slot"
        );

        inPlay.replaceWith(
            flow::Match{core::MatchSettings::forBoardSize(BOARD_SIZE), randomEngine}
        );

        EXPECT_TRUE(inPlay.journal().isEmpty());
        EXPECT_FALSE(inPlay.loadedFrom().has_value());
    }

    TEST(MatchInPlayTests, ALoadedMatchCarriesItsStoryAndItsSave) {
        flow::RandomEngine randomEngine{1U};
        MatchInPlay inPlay{randomEngine};
        flow::MatchEventLog story;
        story.push_back({.kind = flow::MatchEventKind::ShipSunk});

        inPlay.replaceWith(
            flow::Match{core::MatchSettings::forBoardSize(BOARD_SIZE), randomEngine},
            story,
            "20260928-120000"
        );

        EXPECT_EQ(inPlay.journal().entries().size(), 1U);
        EXPECT_EQ(inPlay.loadedFrom(), "20260928-120000");
    }

    TEST(MatchInPlayTests, RemembersTheSlotItWasToldAbout) {
        flow::RandomEngine randomEngine{1U};
        MatchInPlay inPlay{randomEngine};

        inPlay.rememberSlot("20260928-120000");

        EXPECT_EQ(inPlay.loadedFrom(), "20260928-120000");
    }

    TEST(MatchInPlayTests, MovesWhatTheMatchHasToSayIntoTheLog) {
        flow::RandomEngine randomEngine{1U};
        MatchInPlay inPlay{randomEngine};
        inPlay.replaceWith(
            flow::Match{core::MatchSettings::forBoardSize(BOARD_SIZE), randomEngine}
        );
        inPlay.editableMatch().shufflePlayerFleet();
        inPlay.editableMatch().beginBattle();

        inPlay.recordEvents();

        EXPECT_TRUE(inPlay.editableMatch().drainEvents().empty());
    }

    TEST(MatchBehaviorTests, StartingAMatchPutsOneInPlacement) {
        GameFixture fixture;

        fixture.game().play().startNewMatch(BOARD_SIZE);

        EXPECT_TRUE(fixture.game().hasMatch());
        EXPECT_EQ(fixture.game().match().phase(), flow::MatchPhase::Placement);
        EXPECT_EQ(fixture.game().match().settings().boardSize(), BOARD_SIZE);
    }

    TEST(MatchBehaviorTests, StartingAgainReplacesWhatWasThere) {
        GameFixture fixture;
        fixture.game().play().startNewMatch(BOARD_SIZE);
        fixture.game().play().shuffleFleet();

        fixture.game().play().startNewMatch(8);

        EXPECT_EQ(fixture.game().match().settings().boardSize(), 8);
        EXPECT_FALSE(fixture.game().match().playerBoard().hasShips());
    }

    TEST(MatchBehaviorTests, DoingAnythingWithNoMatchInPlayIsHarmless) {
        GameFixture fixture;
        behaviors::MatchBehavior& play = fixture.game().play();

        EXPECT_NO_THROW(play.placeShip({0, 0}, core::Direction::Horizontal, 2));
        EXPECT_NO_THROW(play.removeShipAt({0, 0}));
        EXPECT_NO_THROW(play.shuffleFleet());
        EXPECT_NO_THROW(play.fireAt({0, 0}));
        EXPECT_NO_THROW(play.useSkill(std::nullopt));
        EXPECT_FALSE(play.beginBattle());
        EXPECT_FALSE(fixture.game().hasMatch());
    }

    TEST(MatchBehaviorTests, LaysAShipOut) {
        GameFixture fixture;
        fixture.game().play().startNewMatch(BOARD_SIZE);

        fixture.game().play().placeShip({0, 0}, core::Direction::Horizontal, 3);

        EXPECT_EQ(fixture.game().match().playerBoard().ships().size(), 1U);
    }

    TEST(MatchBehaviorTests, AnIllegalPlacementLeavesTheBoardAlone) {
        GameFixture fixture;
        fixture.game().play().startNewMatch(BOARD_SIZE);

        fixture.game().play().placeShip({BOARD_SIZE, 0}, core::Direction::Horizontal, 3);

        EXPECT_FALSE(fixture.game().match().playerBoard().hasShips());
    }

    TEST(MatchBehaviorTests, TakesAShipBack) {
        GameFixture fixture;
        fixture.game().play().startNewMatch(BOARD_SIZE);
        fixture.game().play().placeShip({0, 0}, core::Direction::Horizontal, 3);

        fixture.game().play().removeShipAt({1, 0});

        EXPECT_FALSE(fixture.game().match().playerBoard().hasShips());
    }

    TEST(MatchBehaviorTests, ShufflingCompletesTheFleet) {
        GameFixture fixture;
        fixture.game().play().startNewMatch(BOARD_SIZE);

        fixture.game().play().shuffleFleet();

        EXPECT_TRUE(fixture.game().match().playerPlacementPlan().isComplete());
    }

    TEST(MatchBehaviorTests, BattleCannotBeginBeforeTheFleetIsLaidOut) {
        GameFixture fixture;
        fixture.game().play().startNewMatch(BOARD_SIZE);

        EXPECT_FALSE(fixture.game().play().beginBattle());

        EXPECT_EQ(fixture.game().match().phase(), flow::MatchPhase::Placement);
    }

    TEST(MatchBehaviorTests, BattleBeginsOnceTheFleetIsLaidOut) {
        GameFixture fixture;
        fixture.game().play().startNewMatch(BOARD_SIZE);
        fixture.game().play().shuffleFleet();

        EXPECT_TRUE(fixture.game().play().beginBattle());

        EXPECT_EQ(fixture.game().match().phase(), flow::MatchPhase::Battle);
    }

    TEST(MatchBehaviorTests, FiringIsWrittenIntoTheJournal) {
        GameFixture fixture;
        fixture.game().play().startNewMatch(BOARD_SIZE);
        fixture.game().play().shuffleFleet();
        fixture.game().play().beginBattle();

        fixture.game().play().fireAt(firstEnemyCell(fixture.game()));

        EXPECT_FALSE(fixture.game().journal().isEmpty());
    }

    TEST(MatchBehaviorTests, FiringSettlesTheTurnSoThePlayerActsNext) {
        GameFixture fixture;
        fixture.game().play().startNewMatch(BOARD_SIZE);
        fixture.game().play().shuffleFleet();
        fixture.game().play().beginBattle();

        fixture.game().play().fireAt({0, 0});

        EXPECT_TRUE(fixture.game().match().isPlayerTurn());
    }

    TEST(MatchBehaviorTests, UsingASkillIsWrittenIntoTheJournal) {
        GameFixture fixture;
        fixture.game().play().startNewMatch(BOARD_SIZE);
        fixture.game().play().shuffleFleet();
        fixture.game().play().beginBattle();
        const std::size_t bankedBefore = fixture.game().match().skills().pending().size();

        fixture.game().play().useSkill(core::Coordinate{1, 1});

        EXPECT_LT(fixture.game().match().skills().pending().size(), bankedBefore);
    }

    TEST(SaveBehaviorTests, StartsWithNothingSaved) {
        GameFixture fixture;

        EXPECT_FALSE(fixture.game().saves().hasSavedMatch());
        EXPECT_TRUE(fixture.game().saves().savedMatches().empty());
        EXPECT_TRUE(fixture.game().saves().nameInPlay().empty());
    }

    TEST(SaveBehaviorTests, SavingWithNoMatchInPlayIsRefused) {
        GameFixture fixture;

        EXPECT_EQ(
            fixture.game().saves().saveMatch("a name"),
            behaviors::SaveOutcome::NoMatchInPlay
        );
        EXPECT_FALSE(fixture.game().saves().hasSavedMatch());
    }

    TEST(SaveBehaviorTests, SavesTheMatchInPlay) {
        GameFixture fixture;
        fixture.game().play().startNewMatch(BOARD_SIZE);

        EXPECT_EQ(fixture.game().saves().saveMatch("my game"), behaviors::SaveOutcome::Saved);

        EXPECT_TRUE(fixture.game().saves().hasSavedMatch());
        ASSERT_EQ(fixture.game().saves().savedMatches().size(), 1U);
        EXPECT_EQ(fixture.game().saves().savedMatches().front().name, "my game");
    }

    TEST(SaveBehaviorTests, ASavedMatchIsNowCalledWhatItWasNamed) {
        GameFixture fixture;
        fixture.game().play().startNewMatch(BOARD_SIZE);
        (void)fixture.game().saves().saveMatch("my game");

        EXPECT_EQ(fixture.game().saves().nameInPlay(), "my game");
    }

    TEST(SaveBehaviorTests, SavingAgainUpdatesTheSameSaveRatherThanAddingOne) {
        GameFixture fixture;
        fixture.game().play().startNewMatch(BOARD_SIZE);
        (void)fixture.game().saves().saveMatch("first name");

        (void)fixture.game().saves().saveMatch("second name");

        ASSERT_EQ(fixture.game().saves().savedMatches().size(), 1U);
        EXPECT_EQ(fixture.game().saves().savedMatches().front().name, "second name");
    }

    TEST(SaveBehaviorTests, StartingAFreshMatchForgetsTheSaveItCameFrom) {
        GameFixture fixture;
        fixture.game().play().startNewMatch(BOARD_SIZE);
        (void)fixture.game().saves().saveMatch("my game");
        const std::string slot = fixture.game().saves().savedMatches().front().id;
        ASSERT_TRUE(fixture.game().saves().loadMatch(slot));
        ASSERT_EQ(fixture.game().saves().nameInPlay(), "my game");

        fixture.game().play().startNewMatch(8);

        EXPECT_TRUE(fixture.game().saves().nameInPlay().empty());
    }

    TEST(SaveBehaviorTests, LoadingASaveThatIsNotThereIsRefused) {
        GameFixture fixture;

        EXPECT_FALSE(fixture.game().saves().loadMatch("missing"));
        EXPECT_FALSE(fixture.game().hasMatch());
    }

    TEST(SaveBehaviorTests, PicksASavedMatchBackUp) {
        GameFixture fixture;
        fixture.game().play().startNewMatch(BOARD_SIZE);
        fixture.game().play().shuffleFleet();
        (void)fixture.game().saves().saveMatch("my game");
        const std::string slot = fixture.game().saves().savedMatches().front().id;
        fixture.game().play().startNewMatch(8);

        EXPECT_TRUE(fixture.game().saves().loadMatch(slot));

        EXPECT_EQ(fixture.game().match().settings().boardSize(), BOARD_SIZE);
        EXPECT_TRUE(fixture.game().match().playerBoard().hasShips());
        EXPECT_EQ(fixture.game().saves().nameInPlay(), "my game");
    }

    TEST(SaveBehaviorTests, ALoadedMatchSavesBackOverItself) {
        GameFixture fixture;
        fixture.game().play().startNewMatch(BOARD_SIZE);
        (void)fixture.game().saves().saveMatch("my game");
        const std::string slot = fixture.game().saves().savedMatches().front().id;
        ASSERT_TRUE(fixture.game().saves().loadMatch(slot));

        (void)fixture.game().saves().saveMatch("renamed");

        ASSERT_EQ(fixture.game().saves().savedMatches().size(), 1U);
        EXPECT_EQ(fixture.game().saves().savedMatches().front().id, slot);
        EXPECT_EQ(fixture.game().saves().savedMatches().front().name, "renamed");
    }

    TEST(SaveBehaviorTests, ALoadedMatchBringsItsStoryWithIt) {
        GameFixture fixture;
        fixture.game().play().startNewMatch(BOARD_SIZE);
        fixture.game().play().shuffleFleet();
        fixture.game().play().beginBattle();
        fixture.game().play().fireAt(firstEnemyCell(fixture.game()));
        ASSERT_FALSE(fixture.game().journal().isEmpty());
        const std::size_t storyLength = fixture.game().journal().entries().size();
        (void)fixture.game().saves().saveMatch("mid battle");
        const std::string slot = fixture.game().saves().savedMatches().front().id;

        fixture.game().play().startNewMatch(8);
        ASSERT_TRUE(fixture.game().saves().loadMatch(slot));

        EXPECT_EQ(fixture.game().journal().entries().size(), storyLength);
    }

    TEST(SaveBehaviorTests, ThrowsASaveAway) {
        GameFixture fixture;
        fixture.game().play().startNewMatch(BOARD_SIZE);
        (void)fixture.game().saves().saveMatch("my game");
        const std::string slot = fixture.game().saves().savedMatches().front().id;

        EXPECT_TRUE(fixture.game().saves().deleteSave(slot));

        EXPECT_FALSE(fixture.game().saves().hasSavedMatch());
    }

    TEST(SaveBehaviorTests, DeletingASaveThatIsNotThereReportsSo) {
        GameFixture fixture;

        EXPECT_FALSE(fixture.game().saves().deleteSave("missing"));
    }

    TEST(WarshipsGameTests, StartsWithNoMatchAndAnEmptyJournal) {
        GameFixture fixture;

        EXPECT_FALSE(fixture.game().hasMatch());
        EXPECT_TRUE(fixture.game().journal().isEmpty());
    }

    TEST(WarshipsGameTests, ReachingForAMatchThatIsNotThereIsRefused) {
        GameFixture fixture;

        EXPECT_THROW((void)fixture.game().match(), errors::NoMatchInPlayException);
    }

    TEST(WarshipsGameTests, BothBehavioursActOnTheOneMatch) {
        GameFixture fixture;

        fixture.game().play().startNewMatch(BOARD_SIZE);

        EXPECT_EQ(fixture.game().saves().saveMatch("a name"), behaviors::SaveOutcome::Saved);
    }

    TEST(WarshipsGameTests, ReadsItsSavesThroughAConstGameToo) {
        GameFixture fixture;
        const WarshipsGame& readOnly = fixture.game();

        EXPECT_FALSE(readOnly.saves().hasSavedMatch());
    }
}  // namespace cpp_warships::model
