#include <iostream>
#include "PauseOngoingGameSubState.h"
#include "ConfigCommandBuilder.h"
#include "DefaultParameterBuilder.h"
#include "DefaultParserError.h"
#include "StateMessages.h"
#include "TypesHelper.h"
#include "ongoing-game-substates/BattleOngoingGameSubState.h"
#include "../../library/ViewHelper.h"

void PauseOngoingGameSubState::handleResume(ParsedOptions options) {
    this->latestCommand = "resume";
}

void PauseOngoingGameSubState::handleLoad(ParsedOptions options) {
    saveCreator = new GameSaveCreator();
    GameStateDTO newDto = saveCreator->loadSave(options["filename"]);
    if (newDto.playerSkillManager == nullptr) {
        newDto.playerSkillManager = new SkillManager(newDto.enemyField, newDto.settings, newDto.enemyManager);
    }
    if (newDto.settings== nullptr) {
        newDto.settings = new MatchSettings(newDto.shipsSizes, newDto.fieldSize);
    }
    newDto.settings->damageCount = 1;
    *dto = newDto;
}

void PauseOngoingGameSubState::handleSave(ParsedOptions options) {
    saveCreator = new GameSaveCreator(dto);
    saveCreator->createSave(options["filename"]);
}

PauseOngoingGameSubState::PauseOngoingGameSubState(SubStateContext &context)
    : OngoingGameSubState(context)
    , dto(context.matchDTO)
{
    ConfigCommandBuilder commandBuilder;
    DefaultParameterBuilder parameterBuilder;
    this->inputScheme = {
        {"resume", ParserCommandInfo(
            commandBuilder
                .setCallback(TypesHelper::methodToFunction(&PauseOngoingGameSubState::handleResume, this))
                .setDescription("Continue the battle")
                .setDisplayError(DefaultParserError::CommandNotFoundError)
                .buildAndReset()
        )},
        {"save", ParserCommandInfo(
                commandBuilder
                .setCallback(TypesHelper::methodToFunction(&PauseOngoingGameSubState::handleSave, this))
                .setDescription("Save the game")
                .setDisplayError(DefaultParserError::CommandNotFoundError)
                .addParameter(
                    parameterBuilder
                        .addFlag("--filename")
                        .setDescription("Write the name of the file with extension")
                        .setValidator(std::regex("^.*\\.json$"))
                        .setNecessary(true)
                        .buildAndReset()
                )
                .buildAndReset()
        )},
        {"load", ParserCommandInfo(
                commandBuilder
                .setCallback(TypesHelper::methodToFunction(&PauseOngoingGameSubState::handleLoad, this))
                .setDescription("Load the game")
                .setDisplayError(DefaultParserError::CommandNotFoundError)
                .addParameter(
                    parameterBuilder
                        .addFlag("--filename")
                        .setDescription("Write the name of the file with extension")
                        .setValidator(std::regex("^.*\\.json$"))
                        .setNecessary(true)
                        .buildAndReset()
                )
                .buildAndReset()
        )}
    };
}

void PauseOngoingGameSubState::openSubState() {
    ViewHelper::consoleOut("\n------------ Battle on paused ------------\n");
}

void PauseOngoingGameSubState::closeSubState() {
    ViewHelper::consoleOut("\n------------ Match resume ------------\n");
}

void PauseOngoingGameSubState::updateSubState() {
    StateMessages::awaitCommandMessage();
    Parser parser(this->inputScheme, DefaultParserError::CommandNotFoundError);
    std::string input;
    std::getline(std::cin, input);
    parser.executedParse(input);
}

OngoingGameSubState* PauseOngoingGameSubState::transitToSubState() {
    if (latestCommand == "resume") {
        return new BattleOngoingGameSubState(context);
    }
    return nullptr;
}