#include "game-states/OngoingGameState.h"
#include "model/StateContext.h"
#include "BattleOngoingGameSubstate.h"

#include "BattleException.h"
#include "DefaultParameterBuilder.h"
#include "DefaultParserError.h"
#include "FinishOngoingGameSubstate.h"
#include "ParserCommandBuilder.h"
#include "StringHelper.h"
#include "TypesHelper.h"
#include "../../library/ViewHelper.h"
#include "parser-builder/ConfigCommandBuilder.h"

BattleOngoingGameSubstate::BattleOngoingGameSubstate(StateContext& context)
    : OngoingGameState(context)
{
    battleController = new BattleController(context);
    ConfigCommandBuilder commandBuilder;
    DefaultParameterBuilder parameterBuilder;
    this->inputScheme = {
        {"attack", ParserCommandInfo(
            commandBuilder
                .setCallback(TypesHelper::methodToFunction(&BattleController::battle,battleController))
                .setDescription("Attack the opponent cell field")
                .setDisplayError(DefaultParserError::WrongFlagValueError)
                .addParameter(
                    parameterBuilder
                        .addFlag("--cell")
                        .setValidator(std::regex("^(\\d+),(\\d+)$"))
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
                .addParameter(
                    parameterBuilder
                        .addFlag("--type")
                        .setValidator(std::regex("^(Scanner|DoubleDamage|Shooting)$"))
                        .setNecessary(true)
                        .buildAndReset()
                )
                .addParameter(
                    parameterBuilder
                        .addFlag("--cell")
                        .setValidator(std::regex("^(\\d+),(\\d+)$"))
                        .setNecessary(false)
                        .setDescription("flag is necessary if you are going to use a scanner")
                        .buildAndReset()
                )
                .buildAndReset()
        )}
    };
}

BattleOngoingGameSubstate::~BattleOngoingGameSubstate() {
    delete battleController;
}

void BattleOngoingGameSubstate::openSubstate() {
    ViewHelper::consoleOut("Welcome to battle. Your target to destroy enemies ships");
}

void BattleOngoingGameSubstate::closeSubstate() {
    ViewHelper::consoleOut("Battle is ended");
}

void BattleOngoingGameSubstate::updateSubstate() {
    Parser parser(this->inputScheme, DefaultParserError::CommandNotFoundError);
    std::string input;
    std::getline(std::cin, input);
    latestCommand = input;
    try {
        parser.executedParse(input);
    }
    catch (const BattleException& exception) {
        exception.displayError();
    }
}

OngoingGameState* BattleOngoingGameSubstate::transitToSubstate() {
    if (battleController->finishBattle()) {
        return new FinishOngoingGameSubstate(context);
    }
    return nullptr;
}