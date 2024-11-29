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


BattleOngoingGameSubState::BattleOngoingGameSubState(SubStateContext& context)
    : OngoingGameSubState(context)
    , battleController(new BattleController(context.matchDTO))
{
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
        std::string input;
        std::getline(std::cin, input);
        parser.executedParse(input);
    } catch (const BattleException& exception) {
        exception.displayError();
    }
}

OngoingGameSubState* BattleOngoingGameSubState::transitToSubState() {
    if (battleController->finishBattle()) {
        return new FinishOngoingGameSubState(context);
    }
    else if (battleController->getPause() == "pause") {
        return new PauseOngoingGameSubState(context);
    }
    return nullptr;
}