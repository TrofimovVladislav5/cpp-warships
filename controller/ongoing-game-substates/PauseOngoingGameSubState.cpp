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
    try {
        loadedSubState = new BattleOngoingGameSubState(context);
    } catch (std::exception &e) {
        ViewHelper::errorOut("Something went wrong while resuming the game", e);
        loadedSubState = nullptr;
    }
}

void PauseOngoingGameSubState::handleLoad(ParsedOptions options) {
    try {
        matchBuilder.loadSave(options["filename"]);
        ViewHelper::consoleOut("Save loaded successfully");

        const bool isForcedLoad = options["force"] == "true";
        const bool isConfirmed = isForcedLoad || ViewHelper::confirmAction("yes");

        if (isConfirmed) {
            loadedSubState = matchBuilder.getStateBuilder()();
        } else {
            loadedSubState = nullptr;
        }
    } catch (std::exception &e) {
        ViewHelper::errorOut("Something went wrong while loading the game", e);
        loadedSubState = nullptr;
    }
}

void PauseOngoingGameSubState::handleSave(ParsedOptions options) {
    try {
        GameSaveCreator saveCreator(context->matchDTO);
        saveCreator.createSave(options["filename"]);
        view.printImportantMessage("Game saved to file: " + options["filename"]);
    } catch (std::exception &e) {
        ViewHelper::errorOut("Something went wrong while saving the game", e);
    }
}

PauseOngoingGameSubState::PauseOngoingGameSubState(SubStateContext* context)
    : OngoingGameSubState(context)
    , matchBuilder(MatchBuilder())
    , view(GamePauseView(30))
    , loadedSubState(nullptr)
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
                .addParameter(
                    parameterBuilder
                        .addFlag("--force")
                        .setDescription("Skip action verification")
                        .setNecessary(false)
                        .setValidator(std::regex("true|false"))
                        .buildAndReset()
                )
                .buildAndReset()
        )}
    };
}

void PauseOngoingGameSubState::openSubState() {
    view.printImportantMessage("Game paused");
}

void PauseOngoingGameSubState::closeSubState() {
    view.printImportantMessage("Match is resumed");
}

void PauseOngoingGameSubState::updateSubState() {
    try {
        StateMessages::awaitCommandMessage();
        Parser parser(this->inputScheme, DefaultParserError::CommandNotFoundError);
        std::string input;
        std::getline(std::cin, input);
        parser.executedParse(input);
    } catch (std::exception &e) {
        ViewHelper::errorOut("Can't parse the inputted command", e);
    }
}

OngoingGameSubState* PauseOngoingGameSubState::transitToSubState() {
    if (loadedSubState) {
        return loadedSubState;
    }

    return nullptr;
}