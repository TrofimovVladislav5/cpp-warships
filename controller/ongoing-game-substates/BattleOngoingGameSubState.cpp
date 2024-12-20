#include "game-states/OngoingGameState.h"
#include "BattleOngoingGameSubState.h"
#include "DefaultParameterBuilder.h"
#include "DefaultParserError.h"
#include "FinishOngoingGameSubState.h"
#include "PauseOngoingGameSubState.h"
#include "StateMessages.h"
#include "TypesHelper.h"
#include "../../library/ViewHelper.h"
#include "exceptions/BattleException.h"
#include "parser-builder/ConfigCommandBuilder.h"
#include "void/VoidParser.h"


OngoingGameSubState *BattleOngoingGameSubState::handleComputerWin() const {
    ViewHelper::consoleOut("Computer won the battle on round " + std::to_string(context->matchDTO->roundNumber));
    return new FinishOngoingGameSubState(context);
}

OngoingGameSubState * BattleOngoingGameSubState::handlePlayerWin() {
    ViewHelper::consoleOut("Player won round number " + std::to_string(context->matchDTO->roundNumber));
    context->matchDTO->roundNumber++;
    enemyPlaceController = new PlaceShipController(context->matchDTO, context->matchDTO->enemyManager);
    enemyPlaceController->placeShipsRandomly();
    context->matchDTO->enemyField->updateShipsCoordinateMap(
        enemyPlaceController->getCurrentField()->getShipsCoordinateMap()
    );

    return new BattleOngoingGameSubState(context);
}

void BattleOngoingGameSubState::handlePause(ParsedOptions options) {
    isPaused = true;
}


BattleOngoingGameSubState::BattleOngoingGameSubState(SubStateContext* context)
    : OngoingGameSubState(context)
    , battleController(new BattleController(context->matchDTO))
    , enemyPlaceController(nullptr)
    , view(BattleView(context->matchDTO))
{
    context->matchDTO->lastSubState = "BattleOngoingGameSubState";

    ConfigCommandBuilder<void> commandBuilder;
    DefaultParameterBuilder parameterBuilder;
    this->inputScheme = {
        {"attack", ParserCommandInfo(
            commandBuilder
                .setCallback(TypesHelper::methodToFunction(&BattleController::battle, battleController))
                .setDescription("Attack the opponent cell field")
                .setDisplayError(DefaultParserError::WrongFlagValueError)
                .addParameter(
                    parameterBuilder
                        .addFlag("--cell")
                        .setValidator(std::regex("^[A-Z][0-9]{1,2}$"))
                        .setNecessary(true)
                        .buildAndReset()
                )
                .buildAndReset()
        )},
        {"skill", ParserCommandInfo(
                commandBuilder
                .setCallback(TypesHelper::methodToFunction(&BattleController::applySkill, battleController))
                .setDescription("Apply the skill")
                .setDisplayError(DefaultParserError::WrongFlagValueError)
                .buildAndReset()
        )},
        {"pause", ParserCommandInfo(
            commandBuilder
                .setCallback(TypesHelper::methodToFunction(&BattleOngoingGameSubState::handlePause, this))
                .setDescription("Pause the battle")
                .setDisplayError(DefaultParserError::WrongFlagValueError)
                .buildAndReset()
        )}
    };
}

BattleOngoingGameSubState::~BattleOngoingGameSubState() {
    delete battleController;
}

void BattleOngoingGameSubState::openSubState() {
    StateMessages::displayGreetingMessage("OngoingGame.Battle");
}

void BattleOngoingGameSubState::closeSubState() {
    ViewHelper::consoleOut("Battle ended");
    StateMessages::displayCloseMessage("OngoingGame.Battle");
}

void BattleOngoingGameSubState::updateSubState() {
    StateMessages::awaitCommandMessage();
    VoidParser parser(this->inputScheme, DefaultParserError::CommandNotFoundError);

    try {
        view.printBattleState();
        std::string input = context->getInputReader()->readCommand();
        parser.executedParse(input);
    } catch (const BattleException& exception) {
        exception.displayError();
    }
}

OngoingGameSubState* BattleOngoingGameSubState::transitToSubState() {
    if (isPaused) {
        return new PauseOngoingGameSubState(context);
    } else if (battleController->getBattleWinner() == BattleWinner::Computer) {
        return handleComputerWin();
    } else if (battleController->getBattleWinner() == BattleWinner::User) {
        return handlePlayerWin();
    }

    return nullptr;
}