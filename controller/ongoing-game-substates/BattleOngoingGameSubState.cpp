#include "game-states/OngoingGameState.h"
#include "BattleOngoingGameSubState.h"
#include "DefaultParameterBuilder.h"
#include "DefaultParserError.h"
#include "NewMatchSettingsSubState.h"
#include "PauseOngoingGameSubState.h"
#include "StateMessages.h"
#include "TypesHelper.h"
#include "../../library/ViewHelper.h"
#include "exceptions/BattleException.h"
#include "parser-builder/ConfigCommandBuilder.h"


OngoingGameSubState *BattleOngoingGameSubState::handleFinishBattle() {
    if (battleController->getCommand() == "computer") {
        ViewHelper::consoleOut("Computer won the battle on round " + std::to_string(context->matchDTO->roundNumber));
        return new NewMatchSettingsSubState(context);
    }

    ViewHelper::consoleOut("Player won the battle round " + std::to_string(context->matchDTO->roundNumber));
    context->matchDTO->roundNumber++;
    enemyPlaceController = new PlaceShipController(context->matchDTO, context->matchDTO->enemyManager);
    enemyPlaceController->placeShipsRandomly();
    context->matchDTO->enemyField->updateShipsCoordinateMap(
        enemyPlaceController->getCurrentField()->getShipsCoordinateMap()
    );
    return new BattleOngoingGameSubState(context);
}


BattleOngoingGameSubState::BattleOngoingGameSubState(SubStateContext* context)
    : OngoingGameSubState(context)
    , battleController(new BattleController(context->matchDTO))
    , enemyPlaceController(nullptr)
    , view(BattleView(context->matchDTO))
{
    context->matchDTO->lastSubState = "BattleOngoingGameSubState";

    ConfigCommandBuilder commandBuilder;
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
                .setCallback(TypesHelper::methodToFunction(&BattleController::pause, battleController))
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
    Parser parser(this->inputScheme, DefaultParserError::CommandNotFoundError);

    try {
        view.printBattleState();
        std::string input;
        std::getline(std::cin, input);
        parser.executedParse(input);
    } catch (const BattleException& exception) {
        exception.displayError();
    }
}

OngoingGameSubState* BattleOngoingGameSubState::transitToSubState() {
    if (battleController->getCommand() == "pause") {
        return new PauseOngoingGameSubState(context);
    }
    else if (battleController->finishBattle()) {
        return handleFinishBattle();
    }
    return nullptr;
}