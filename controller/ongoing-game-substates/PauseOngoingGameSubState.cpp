#include <iostream>
#include "PauseOngoingGameSubState.h"
#include "ConfigCommandBuilder.h"
#include "DefaultParameterBuilder.h"
#include "DefaultParserError.h"
#include "InitiateOngoingGameSubState.h"
#include "StateMessages.h"
#include "TypesHelper.h"
#include "ongoing-game-substates/BattleOngoingGameSubState.h"
#include "../../library/ViewHelper.h"

void PauseOngoingGameSubState::handleResume(ParsedOptions options) {
    latestCommand = "resume";
}

void PauseOngoingGameSubState::handleLoad(ParsedOptions options) {
    latestCommand = "load";
    saveCreator = new GameSaveCreator();
    dto = saveCreator->loadSave(options["filename"]);
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
    ViewHelper::consoleOut("\n------------ Game on paused ------------\n");
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
    if (latestCommand == "load") {
        if (dto->lastSubState == "InitiateOngoingGameSubState") {
            return new InitiateOngoingGameSubState(context);
        }
        else if (dto->lastSubState == "BattleOngoingGameSubState") {
            return new BattleOngoingGameSubState(context);
        }
    }

    if (latestCommand == "resume") {
        return new BattleOngoingGameSubState(context);
    }

    return nullptr;
}