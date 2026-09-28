#include <application/core/Board.h>
#include <application/core/Coordinate.h>
#include <application/core/Direction.h>
#include <application/core/FleetComposition.h>
#include <application/core/MatchSettings.h>
#include <application/core/Outcomes.h>
#include <application/core/Ship.h>
#include <application/flow/Match.h>
#include <application/flow/MatchEvent.h>
#include <application/flow/MatchPhase.h>
#include <application/flow/Participant.h>
#include <application/flow/RandomEngine.h>
#include <application/flow/SkillKind.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <cstddef>
#include <deque>
#include <map>
#include <optional>
#include <vector>

namespace cpp_warships::flow {
    namespace {
        constexpr unsigned int FIXED_SEED = 777U;
        constexpr int BOARD_SIZE = 8;

        /** @brief A small fleet of one-cell ships, so a shot settles a ship outright. */
        core::MatchSettings fragileSettings(int segmentHealth = 1) {
            return core::MatchSettings{
                BOARD_SIZE,
                core::FleetComposition{std::map<int, int>{{1, 2}}},
                1,
                segmentHealth
            };
        }

        /** @brief Every cell the enemy fleet currently stands on. */
        std::vector<core::Coordinate> enemyCells(const Match& match) {
            std::vector<core::Coordinate> cells;
            for (const core::Ship& ship : match.computerBoard().ships()) {
                for (const core::Coordinate cell : ship.coordinates()) {
                    cells.push_back(cell);
                }
            }

            return cells;
        }

        /** @brief Takes a match through placement into battle. */
        void startBattle(Match& match) {
            ASSERT_TRUE(match.shufflePlayerFleet());
            ASSERT_TRUE(match.beginBattle());
        }

        /** @brief Whether @p log holds an event of @p kind. */
        bool holds(const MatchEventLog& log, MatchEventKind kind) {
            const auto isKind = [kind](const MatchEvent& event) { return event.kind == kind; };
            return std::any_of(log.begin(), log.end(), isKind);
        }
    }  // namespace

    TEST(MatchTests, OpensInPlacementOnTheFirstRound) {
        RandomEngine randomEngine{FIXED_SEED};
        const Match match{fragileSettings(), randomEngine};

        EXPECT_EQ(match.phase(), MatchPhase::Placement);
        EXPECT_EQ(match.roundNumber(), 1);
        EXPECT_TRUE(match.isPlayerTurn());
        EXPECT_EQ(match.currentTurn(), Participant::Player);
    }

    TEST(MatchTests, OpensWithBoardsSizedToTheSettings) {
        RandomEngine randomEngine{FIXED_SEED};
        const Match match{fragileSettings(), randomEngine};

        EXPECT_EQ(match.playerBoard().width(), BOARD_SIZE);
        EXPECT_EQ(match.computerBoard().height(), BOARD_SIZE);
        EXPECT_FALSE(match.playerBoard().hasShips());
        EXPECT_FALSE(match.computerBoard().hasShips());
    }

    TEST(MatchTests, OpensWithAHandOfEverySkill) {
        RandomEngine randomEngine{FIXED_SEED};
        const Match match{fragileSettings(), randomEngine};

        EXPECT_EQ(match.skills().pending().size(), ALL_SKILL_KINDS.size());
    }

    TEST(MatchTests, KeepsTheSettingsItWasGiven) {
        RandomEngine randomEngine{FIXED_SEED};
        const Match match{fragileSettings(3), randomEngine};

        EXPECT_EQ(match.settings().boardSize(), BOARD_SIZE);
        EXPECT_EQ(match.settings().segmentHealth(), 3);
    }

    TEST(MatchTests, TheFleetOwedFollowsThePlayerBoard) {
        RandomEngine randomEngine{FIXED_SEED};
        Match match{fragileSettings(), randomEngine};

        EXPECT_EQ(match.playerPlacementPlan().remainingShipCount(), 2);

        match.editablePlayerBoard().place({0, 0}, core::Direction::Horizontal, 1);

        EXPECT_EQ(match.playerPlacementPlan().remainingShipCount(), 1);
    }

    TEST(MatchTests, ShufflingCompletesThePlayerFleet) {
        RandomEngine randomEngine{FIXED_SEED};
        Match match{fragileSettings(), randomEngine};

        EXPECT_TRUE(match.shufflePlayerFleet());

        EXPECT_TRUE(match.playerPlacementPlan().isComplete());
    }

    TEST(MatchTests, ShufflingDiscardsTheOldLayout) {
        RandomEngine randomEngine{FIXED_SEED};
        Match match{fragileSettings(), randomEngine};
        match.editablePlayerBoard().place({0, 0}, core::Direction::Horizontal, 1);
        match.editablePlayerBoard().place({2, 0}, core::Direction::Horizontal, 1);
        match.editablePlayerBoard().place({4, 0}, core::Direction::Horizontal, 1);

        match.shufflePlayerFleet();

        EXPECT_EQ(match.playerBoard().ships().size(), 2U);
    }

    TEST(MatchTests, BattleCannotBeginWithAnIncompleteFleet) {
        RandomEngine randomEngine{FIXED_SEED};
        Match match{fragileSettings(), randomEngine};

        EXPECT_FALSE(match.beginBattle());

        EXPECT_EQ(match.phase(), MatchPhase::Placement);
        EXPECT_FALSE(match.computerBoard().hasShips());
    }

    TEST(MatchTests, BeginningBattleLaysOutTheEnemyFleet) {
        RandomEngine randomEngine{FIXED_SEED};
        Match match{fragileSettings(), randomEngine};
        startBattle(match);

        EXPECT_EQ(match.phase(), MatchPhase::Battle);
        EXPECT_EQ(match.computerBoard().ships().size(), 2U);
        EXPECT_TRUE(match.isPlayerTurn());
    }

    TEST(MatchTests, BattleCannotBeginTwice) {
        RandomEngine randomEngine{FIXED_SEED};
        Match match{fragileSettings(), randomEngine};
        startBattle(match);

        EXPECT_FALSE(match.beginBattle());
    }

    TEST(MatchTests, FiringDuringPlacementIsRefused) {
        RandomEngine randomEngine{FIXED_SEED};
        Match match{fragileSettings(), randomEngine};

        EXPECT_EQ(match.fireAt({0, 0}), core::AttackOutcome::AlreadyAttacked);
    }

    TEST(MatchTests, AHitKeepsTheTurn) {
        RandomEngine randomEngine{FIXED_SEED};
        Match match{fragileSettings(2), randomEngine};
        startBattle(match);

        EXPECT_EQ(match.fireAt(enemyCells(match).front()), core::AttackOutcome::Hit);

        EXPECT_TRUE(match.isPlayerTurn());
    }

    TEST(MatchTests, AHitIsWrittenIntoTheHistory) {
        RandomEngine randomEngine{FIXED_SEED};
        Match match{fragileSettings(2), randomEngine};
        startBattle(match);
        match.drainEvents();

        const core::Coordinate target = enemyCells(match).front();
        match.fireAt(target);

        const MatchEventLog events = match.drainEvents();
        ASSERT_FALSE(events.empty());
        EXPECT_EQ(events.front().kind, MatchEventKind::ShipDamaged);
        EXPECT_EQ(events.front().actor, Participant::Player);
        EXPECT_EQ(events.front().coordinate, target);
    }

    TEST(MatchTests, SinkingAShipEarnsASkill) {
        RandomEngine randomEngine{FIXED_SEED};
        Match match{fragileSettings(), randomEngine};
        startBattle(match);
        const std::size_t bankedBefore = match.skills().pending().size();
        match.drainEvents();

        EXPECT_EQ(match.fireAt(enemyCells(match).front()), core::AttackOutcome::Sunk);

        EXPECT_EQ(match.skills().pending().size(), bankedBefore + 1);
        const MatchEventLog events = match.drainEvents();
        EXPECT_TRUE(holds(events, MatchEventKind::ShipSunk));
        EXPECT_TRUE(holds(events, MatchEventKind::SkillGranted));
    }

    TEST(MatchTests, AMissHandsTheTurnOver) {
        RandomEngine randomEngine{FIXED_SEED};
        Match match{fragileSettings(), randomEngine};
        startBattle(match);
        match.drainEvents();

        std::optional<core::Coordinate> emptyCell;
        for (int row = 0; row < BOARD_SIZE && !emptyCell.has_value(); ++row) {
            for (int column = 0; column < BOARD_SIZE && !emptyCell.has_value(); ++column) {
                const core::Coordinate candidate{column, row};
                const bool isShip =
                    match.computerBoard().stateAt(candidate, core::Visibility::Owner) ==
                    core::CellState::Ship;
                if (!isShip) {
                    emptyCell = candidate;
                }
            }
        }
        ASSERT_TRUE(emptyCell.has_value());

        EXPECT_EQ(match.fireAt(*emptyCell), core::AttackOutcome::Miss);

        EXPECT_FALSE(match.isPlayerTurn());
        const MatchEventLog events = match.drainEvents();
        EXPECT_TRUE(holds(events, MatchEventKind::ShotMissed));
        EXPECT_TRUE(holds(events, MatchEventKind::TurnPassed));
    }

    TEST(MatchTests, FiringOutOfTurnIsRefused) {
        RandomEngine randomEngine{FIXED_SEED};
        Match match{fragileSettings(), randomEngine};
        startBattle(match);
        match.fireAt({-1, -1});

        EXPECT_EQ(match.fireAt({-1, -1}), core::AttackOutcome::OutOfBounds);
    }

    TEST(MatchTests, ARejectedShotCostsNeitherTurnNorBonus) {
        RandomEngine randomEngine{FIXED_SEED};
        Match match{fragileSettings(), randomEngine};
        startBattle(match);

        EXPECT_EQ(match.fireAt({BOARD_SIZE, BOARD_SIZE}), core::AttackOutcome::OutOfBounds);

        EXPECT_TRUE(match.isPlayerTurn());
    }

    TEST(MatchTests, ClearingTheEnemyFleetStartsTheNextRound) {
        RandomEngine randomEngine{FIXED_SEED};
        Match match{fragileSettings(), randomEngine};
        startBattle(match);
        match.drainEvents();

        for (const core::Coordinate cell : enemyCells(match)) {
            match.fireAt(cell);
        }

        EXPECT_EQ(match.roundNumber(), 2);
        EXPECT_EQ(match.phase(), MatchPhase::Battle);
        EXPECT_TRUE(match.isPlayerTurn());
        EXPECT_TRUE(holds(match.drainEvents(), MatchEventKind::RoundWon));
    }

    TEST(MatchTests, ANewRoundBringsAFreshEnemyFleet) {
        RandomEngine randomEngine{FIXED_SEED};
        Match match{fragileSettings(), randomEngine};
        startBattle(match);

        for (const core::Coordinate cell : enemyCells(match)) {
            match.fireAt(cell);
        }

        EXPECT_EQ(match.computerBoard().ships().size(), 2U);
        EXPECT_FALSE(match.computerBoard().allShipsSunk());
    }

    TEST(MatchTests, TheComputerDoesNotPlayDuringPlacement) {
        RandomEngine randomEngine{FIXED_SEED};
        Match match{fragileSettings(), randomEngine};

        match.runComputerTurn();

        EXPECT_TRUE(match.drainEvents().empty());
    }

    TEST(MatchTests, TheComputerDoesNotPlayOnThePlayersTurn) {
        RandomEngine randomEngine{FIXED_SEED};
        Match match{fragileSettings(), randomEngine};
        startBattle(match);
        match.drainEvents();

        match.runComputerTurn();

        EXPECT_TRUE(match.drainEvents().empty());
    }

    TEST(MatchTests, ConcludingOnThePlayersTurnChangesNothing) {
        RandomEngine randomEngine{FIXED_SEED};
        Match match{fragileSettings(), randomEngine};
        startBattle(match);
        match.drainEvents();

        match.concludeTurn();

        EXPECT_TRUE(match.isPlayerTurn());
        EXPECT_TRUE(match.drainEvents().empty());
    }

    TEST(MatchTests, TheComputerFiresAndHandsTheTurnBack) {
        RandomEngine randomEngine{FIXED_SEED};
        Match match{core::MatchSettings::forBoardSize(BOARD_SIZE), randomEngine};
        startBattle(match);
        std::optional<core::Coordinate> emptyCell;
        for (int row = 0; row < BOARD_SIZE && !emptyCell.has_value(); ++row) {
            for (int column = 0; column < BOARD_SIZE && !emptyCell.has_value(); ++column) {
                const core::Coordinate candidate{column, row};
                const bool isShip =
                    match.computerBoard().stateAt(candidate, core::Visibility::Owner) ==
                    core::CellState::Ship;
                if (!isShip) {
                    emptyCell = candidate;
                }
            }
        }
        ASSERT_TRUE(emptyCell.has_value());
        match.fireAt(*emptyCell);
        ASSERT_FALSE(match.isPlayerTurn());
        match.drainEvents();

        match.concludeTurn();

        EXPECT_TRUE(match.isPlayerTurn());
        EXPECT_FALSE(match.playerBoard().attackedCells().empty());
        EXPECT_TRUE(holds(match.drainEvents(), MatchEventKind::TurnPassed));
    }

    TEST(MatchTests, ASkillCannotBeAppliedDuringPlacement) {
        RandomEngine randomEngine{FIXED_SEED};
        Match match{fragileSettings(), randomEngine};

        EXPECT_FALSE(match.applyNextSkill());
    }

    TEST(MatchTests, TheScannerIsRefusedWithoutATarget) {
        RandomEngine randomEngine{FIXED_SEED};
        MatchRestoreState state{
            .playerBoard = core::Board{BOARD_SIZE, BOARD_SIZE},
            .computerBoard = core::Board{BOARD_SIZE, BOARD_SIZE},
            .bankedSkills = std::deque<SkillKind>{SkillKind::Scanner},
            .phase = MatchPhase::Battle
        };
        Match match{fragileSettings(), randomEngine, std::move(state)};

        EXPECT_TRUE(match.nextSkillNeedsTarget());
        EXPECT_FALSE(match.applyNextSkill());
        EXPECT_EQ(match.skills().next(), SkillKind::Scanner);
    }

    TEST(MatchTests, TheScannerReportsWhatItFinds) {
        RandomEngine randomEngine{FIXED_SEED};
        core::Board enemyBoard{BOARD_SIZE, BOARD_SIZE};
        enemyBoard.place({4, 4}, core::Direction::Horizontal, 1);
        MatchRestoreState state{
            .playerBoard = core::Board{BOARD_SIZE, BOARD_SIZE},
            .computerBoard = std::move(enemyBoard),
            .bankedSkills = std::deque<SkillKind>{SkillKind::Scanner},
            .phase = MatchPhase::Battle
        };
        Match match{fragileSettings(), randomEngine, std::move(state)};

        EXPECT_TRUE(match.applyNextSkill(core::Coordinate{4, 4}));

        const MatchEventLog events = match.drainEvents();
        ASSERT_EQ(events.size(), 1U);
        EXPECT_EQ(events.front().kind, MatchEventKind::AreaScanned);
        EXPECT_TRUE(events.front().scanFoundShip);
        EXPECT_TRUE(match.skills().isEmpty());
    }

    TEST(MatchTests, DoubleDamageArmsTheNextShot) {
        RandomEngine randomEngine{FIXED_SEED};
        MatchRestoreState state{
            .playerBoard = core::Board{BOARD_SIZE, BOARD_SIZE},
            .computerBoard = core::Board{BOARD_SIZE, BOARD_SIZE},
            .bankedSkills = std::deque<SkillKind>{SkillKind::DoubleDamage},
            .phase = MatchPhase::Battle
        };
        Match match{fragileSettings(), randomEngine, std::move(state)};

        EXPECT_FALSE(match.nextSkillNeedsTarget());
        EXPECT_TRUE(match.applyNextSkill());

        EXPECT_TRUE(match.isDoubleDamageArmed());
        EXPECT_TRUE(holds(match.drainEvents(), MatchEventKind::DoubleDamageArmed));
    }

    TEST(MatchTests, AnArmedShotDealsDoubleDamage) {
        RandomEngine randomEngine{FIXED_SEED};
        core::Board enemyBoard{BOARD_SIZE, BOARD_SIZE};
        enemyBoard.place({4, 4}, core::Direction::Horizontal, 1, 2);
        MatchRestoreState state{
            .playerBoard = core::Board{BOARD_SIZE, BOARD_SIZE},
            .computerBoard = std::move(enemyBoard),
            .bankedSkills = std::deque<SkillKind>{SkillKind::DoubleDamage},
            .phase = MatchPhase::Battle
        };
        Match match{fragileSettings(2), randomEngine, std::move(state)};
        match.applyNextSkill();

        EXPECT_EQ(match.fireAt({4, 4}), core::AttackOutcome::Sunk);

        EXPECT_FALSE(match.isDoubleDamageArmed());
    }

    TEST(MatchTests, ARandomStrikeFiresWithoutEndingTheTurn) {
        RandomEngine randomEngine{FIXED_SEED};
        MatchRestoreState state{
            .playerBoard = core::Board{BOARD_SIZE, BOARD_SIZE},
            .computerBoard = core::Board{BOARD_SIZE, BOARD_SIZE},
            .bankedSkills = std::deque<SkillKind>{SkillKind::RandomStrike},
            .phase = MatchPhase::Battle
        };
        Match match{fragileSettings(), randomEngine, std::move(state)};

        EXPECT_TRUE(match.applyNextSkill());

        EXPECT_TRUE(match.isPlayerTurn());
        EXPECT_EQ(match.computerBoard().attackedCells().size(), 1U);
    }

    TEST(MatchTests, ResumesExactlyWhereASaveLeftOff) {
        RandomEngine randomEngine{FIXED_SEED};
        core::Board playerBoard{BOARD_SIZE, BOARD_SIZE};
        playerBoard.place({0, 0}, core::Direction::Horizontal, 1);
        AiMemory opponentMemory;
        opponentMemory.attemptedCoordinates = {{5, 5}};

        MatchRestoreState state{
            .playerBoard = std::move(playerBoard),
            .computerBoard = core::Board{BOARD_SIZE, BOARD_SIZE},
            .bankedSkills = std::deque<SkillKind>{SkillKind::Scanner},
            .roundNumber = 4,
            .phase = MatchPhase::Battle,
            .currentTurn = Participant::Computer,
            .isDoubleDamageArmed = true,
            .opponentMemory = opponentMemory
        };
        const Match match{fragileSettings(), randomEngine, std::move(state)};

        EXPECT_EQ(match.roundNumber(), 4);
        EXPECT_EQ(match.phase(), MatchPhase::Battle);
        EXPECT_EQ(match.currentTurn(), Participant::Computer);
        EXPECT_TRUE(match.isDoubleDamageArmed());
        EXPECT_EQ(match.skills().next(), SkillKind::Scanner);
        EXPECT_TRUE(match.playerBoard().hasShips());
        EXPECT_TRUE(match.opponentMemory().attemptedCoordinates.contains({5, 5}));
    }

    TEST(MatchTests, AResumedMatchIsNotDealtAFreshHand) {
        RandomEngine randomEngine{FIXED_SEED};
        MatchRestoreState state{
            .playerBoard = core::Board{BOARD_SIZE, BOARD_SIZE},
            .computerBoard = core::Board{BOARD_SIZE, BOARD_SIZE}
        };
        const Match match{fragileSettings(), randomEngine, std::move(state)};

        EXPECT_TRUE(match.skills().isEmpty());
    }

    TEST(MatchTests, LosingEveryShipEndsTheMatch) {
        RandomEngine randomEngine{FIXED_SEED};
        core::Board playerBoard{1, 1};
        playerBoard.place({0, 0}, core::Direction::Horizontal, 1, 1);
        MatchRestoreState state{
            .playerBoard = std::move(playerBoard),
            .computerBoard = core::Board{BOARD_SIZE, BOARD_SIZE},
            .phase = MatchPhase::Battle,
            .currentTurn = Participant::Computer
        };
        Match match{fragileSettings(), randomEngine, std::move(state)};

        match.runComputerTurn();

        EXPECT_EQ(match.phase(), MatchPhase::Finished);
        EXPECT_TRUE(match.playerBoard().allShipsSunk());
        EXPECT_TRUE(holds(match.drainEvents(), MatchEventKind::MatchLost));
    }

    TEST(MatchTests, DrainingHandsOverTheHistoryAndClearsIt) {
        RandomEngine randomEngine{FIXED_SEED};
        Match match{fragileSettings(), randomEngine};
        startBattle(match);
        match.fireAt(enemyCells(match).front());

        EXPECT_FALSE(match.drainEvents().empty());
        EXPECT_TRUE(match.drainEvents().empty());
    }

    TEST(MatchTests, TheOpponentsMemoryGrowsAsItPlays) {
        RandomEngine randomEngine{FIXED_SEED};
        Match match{core::MatchSettings::forBoardSize(BOARD_SIZE), randomEngine};
        startBattle(match);
        EXPECT_TRUE(match.opponentMemory().attemptedCoordinates.empty());

        MatchRestoreState state{
            .playerBoard = core::Board{BOARD_SIZE, BOARD_SIZE},
            .computerBoard = core::Board{BOARD_SIZE, BOARD_SIZE},
            .phase = MatchPhase::Battle,
            .currentTurn = Participant::Computer
        };
        Match computerToPlay{fragileSettings(), randomEngine, std::move(state)};
        computerToPlay.runComputerTurn();

        EXPECT_FALSE(computerToPlay.opponentMemory().attemptedCoordinates.empty());
    }

    TEST(MatchTests, TheSameSeedPlaysTheSameMatch) {
        RandomEngine firstEngine{FIXED_SEED};
        RandomEngine secondEngine{FIXED_SEED};
        Match firstMatch{fragileSettings(), firstEngine};
        Match secondMatch{fragileSettings(), secondEngine};

        startBattle(firstMatch);
        startBattle(secondMatch);

        EXPECT_EQ(enemyCells(firstMatch), enemyCells(secondMatch));
        EXPECT_EQ(firstMatch.skills().pending(), secondMatch.skills().pending());
    }
}  // namespace cpp_warships::flow
