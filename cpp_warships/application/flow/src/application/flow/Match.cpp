#include <application/flow/Match.h>

#include <utility>

namespace cpp_warships::flow {
    Match::Match(core::MatchSettings settings, RandomEngine& randomEngine)
        : settings_(std::move(settings))
        , randomEngine_(randomEngine)
        , playerBoard_(settings_.boardSize(), settings_.boardSize())
        , computerBoard_(settings_.boardSize(), settings_.boardSize())
        , aiOpponent_(randomEngine)
        , skillManager_(randomEngine)
        , shotStrength_(settings_.baseDamage())
        , turnOrder_(Participant::Player)
        , phase_(MatchPhase::Placement)
        , roundNumber_(1) {
        skillManager_.grantOpeningHand();
    }

    Match::Match(core::MatchSettings settings, RandomEngine& randomEngine, MatchRestoreState state)
        : settings_(std::move(settings))
        , randomEngine_(randomEngine)
        , playerBoard_(std::move(state.playerBoard))
        , computerBoard_(std::move(state.computerBoard))
        , aiOpponent_(randomEngine, std::move(state.opponentMemory))
        , skillManager_(randomEngine, SkillQueue{std::move(state.bankedSkills)})
        , shotStrength_(settings_.baseDamage(), state.isDoubleDamageArmed)
        , turnOrder_(state.currentTurn)
        , phase_(state.phase)
        , roundNumber_(state.roundNumber) {}

    const core::MatchSettings& Match::settings() const noexcept {
        return settings_;
    }

    MatchPhase Match::phase() const noexcept {
        return phase_;
    }

    int Match::roundNumber() const noexcept {
        return roundNumber_;
    }

    const core::Board& Match::playerBoard() const noexcept {
        return playerBoard_;
    }

    const core::Board& Match::computerBoard() const noexcept {
        return computerBoard_;
    }

    Participant Match::currentTurn() const noexcept {
        return turnOrder_.current();
    }

    bool Match::isDoubleDamageArmed() const noexcept {
        return shotStrength_.isDoubleDamageArmed();
    }

    core::Board& Match::editablePlayerBoard() noexcept {
        return playerBoard_;
    }

    PlacementPlan Match::playerPlacementPlan() const {
        return PlacementPlan{settings_.fleet(), playerBoard_};
    }

    bool Match::shufflePlayerFleet() {
        return placeFleetRandomly(
            playerBoard_,
            settings_.fleet(),
            randomEngine_,
            settings_.segmentHealth()
        );
    }

    bool Match::beginBattle() {
        if (phase_ != MatchPhase::Placement || !playerPlacementPlan().isComplete()) {
            return false;
        } else if (!placeFleetRandomly(
            computerBoard_,
            settings_.fleet(),
            randomEngine_,
            settings_.segmentHealth()
        )) {
            return false;
        }

        phase_ = MatchPhase::Battle;
        turnOrder_.giveTo(Participant::Player);
        return true;
    }

    bool Match::isPlayerTurn() const noexcept {
        return turnOrder_.isPlayerTurn();
    }

    void Match::startNextRound() {
        events_.record(
            {
                .kind = MatchEventKind::RoundWon,
                .actor = Participant::Player
            }
        );

        placeFleetRandomly(
            computerBoard_,
            settings_.fleet(),
            randomEngine_,
            settings_.segmentHealth()
        );
        ++roundNumber_;
        turnOrder_.giveTo(Participant::Player);
    }

    void Match::concludeAsLoss() {
        events_.record(
            {
                .kind = MatchEventKind::MatchLost,
                .actor = Participant::Computer
            }
        );
        phase_ = MatchPhase::Finished;
    }

    void
    Match::recordPlayerShot(const core::AttackOutcome outcome, const core::Coordinate coordinate) {
        const AttackOutcomeBehaviour& behaviour = behaviourFor(outcome);
        events_.record(
            {
                .kind = behaviour.eventKind(),
                .actor = Participant::Player,
                .coordinate = coordinate
            }
        );

        if (behaviour.grantsSkill()) {
            const SkillKind granted = skillManager_.grantRandom();
            events_.record(
                {
                    .kind = MatchEventKind::SkillGranted,
                     .actor = Participant::Player,
                     .skill = granted
                }
            );
        }
    }

    void Match::passTurnUnlessKept(const core::AttackOutcome outcome) {
        if (behaviourFor(outcome).keepsTurn()) {
            return;
        }

        turnOrder_.pass();
        events_.record(
            {
                .kind = MatchEventKind::TurnPassed,
                .actor = turnOrder_.current()
            }
        );
    }

    core::AttackOutcome Match::fireAt(core::Coordinate coordinate) {
        if (phase_ != MatchPhase::Battle || !turnOrder_.isPlayerTurn()) {
            return core::AttackOutcome::AlreadyAttacked;
        }

        const core::AttackOutcome outcome = computerBoard_.attack(
            coordinate,
            shotStrength_.nextShotDamage()
        );
        if (behaviourFor(outcome).isShotSpent()) {
            shotStrength_.spend();
        }

        recordPlayerShot(outcome, coordinate);
        passTurnUnlessKept(outcome);
        if (computerBoard_.allShipsSunk()) {
            startNextRound();
        }

        return outcome;
    }

    bool Match::takeComputerShot() {
        const std::optional<core::Coordinate> targetCell = aiOpponent_.chooseTarget(playerBoard_);
        bool keepsTurn = false;

        if (targetCell.has_value()) {
            const core::AttackOutcome outcome = playerBoard_.attack(
                *targetCell,
                settings_.baseDamage()
            );
            aiOpponent_.recordOutcome(*targetCell, outcome, playerBoard_);

            const AttackOutcomeBehaviour& behaviour = behaviourFor(outcome);
            keepsTurn = behaviour.keepsTurn() && behaviour.isShotSpent();
            events_.record(
                {
                    .kind = behaviour.eventKind(),
                     .actor = Participant::Computer,
                     .coordinate = *targetCell
                }
            );
        }

        return keepsTurn;
    }

    void Match::runComputerTurn() {
        if (phase_ != MatchPhase::Battle || turnOrder_.isPlayerTurn()) {
            return;
        }

        bool keepsTurn = true;
        bool isPlayerFleetSunk = false;
        while (keepsTurn && !isPlayerFleetSunk) {
            keepsTurn = takeComputerShot();
            isPlayerFleetSunk = playerBoard_.allShipsSunk();
        }

        if (isPlayerFleetSunk) {
            concludeAsLoss();
        } else {
            turnOrder_.giveTo(Participant::Player);
            events_.record(
                {
                    .kind = MatchEventKind::TurnPassed,
                    .actor = Participant::Player
                }
            );
        }
    }

    void Match::concludeTurn() {
        if (!turnOrder_.isPlayerTurn()) {
            runComputerTurn();
        }
    }

    AiMemory Match::opponentMemory() const {
        return aiOpponent_.memory();
    }

    const SkillQueue& Match::skills() const noexcept {
        return skillManager_.bank();
    }

    bool Match::nextSkillNeedsTarget() const {
        return skillManager_.nextNeedsTarget();
    }

    bool Match::applyNextSkill(std::optional<core::Coordinate> scanTarget) {
        if (phase_ != MatchPhase::Battle || !turnOrder_.isPlayerTurn()) {
            return false;
        }

        return skillManager_.applyNext(*this, scanTarget);
    }

    const core::Board& Match::enemyBoard() const {
        return computerBoard_;
    }

    RandomEngine& Match::randomEngine() {
        return randomEngine_;
    }

    void Match::armDoubleDamage() {
        shotStrength_.armDoubleDamage();
    }

    void Match::strikeEnemyCell(core::Coordinate coordinate) {
        const core::AttackOutcome outcome = computerBoard_.attack(
            coordinate,
            shotStrength_.baseDamage()
        );

        recordPlayerShot(outcome, coordinate);
        if (computerBoard_.allShipsSunk()) {
            startNextRound();
        }
    }

    void Match::recordSkillEvent(const MatchEvent& event) {
        events_.record(event);
    }

    MatchEventLog Match::drainEvents() {
        return events_.drain();
    }
}  // namespace cpp_warships::flow
