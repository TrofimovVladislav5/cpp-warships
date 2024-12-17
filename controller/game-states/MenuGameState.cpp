#include "MenuGameState.h"

#include <iostream>

#include "DefaultParameterBuilder.h"
#include "GameState.h"
#include "OngoingGameState.h"
#include "StateMessages.h"
#include "ViewHelper.h"
#include "library/TypesHelper.h"
#include "library/parser-builder/ConfigCommandBuilder.h"
#include "library/defaults/DefaultParserError.h"


void MenuGameState::handleGameLoad(ParsedOptions options) {
    std::string filename = options["filename"];

    if (this->matchBuilder->loadSave(filename)) {
        ViewHelper::consoleOut("Successfully read file from " + filename);
    }
}

void MenuGameState::handleNewGame(ParsedOptions options) {
    bool isDefault = options["default"] == "true";
    this->matchBuilder->newGame(isDefault);
    ViewHelper::consoleOut("Successfully initialized new game");
}

void MenuGameState::handleConfirm(ParsedOptions options) {
    isRunning = true;
}

MenuGameState::MenuGameState(StateContext& context) 
    : GameState(context)
    , matchBuilder(new MatchBuilder())
    , latestCommand("")
{
    ConfigCommandBuilder commandBuilder;
    DefaultParameterBuilder parameterBuilder;
    inputScheme = {
        {"load", ParserCommandInfo(
            commandBuilder
                .setCallback(TypesHelper::methodToFunction(&MenuGameState::handleGameLoad, this))
                .setDescription("Load a game from a file")
                .setDisplayError(DefaultParserError::WrongFlagValueError)
                .addParameter(
                    parameterBuilder
                        .addFlag("--filename")
                        .setDescription("Specify the file name to load the game from. Make sure it's a .json file")
                        .setValidator(std::regex("^.*\\.json$"))
                        .setNecessary(true)
                        .buildAndReset()
                )
                .buildAndReset()
            )
        },
        {"new", ParserCommandInfo(
            commandBuilder
                .setCallback(TypesHelper::methodToFunction(&MenuGameState::handleNewGame, this))
                .setDescription("Start new game from scratch")
                .setDisplayError(DefaultParserError::WrongFlagValueError)
                .addParameter(
                    parameterBuilder
                        .addFlag("--default")
                        .setDescription("Start game with default settings and skip the initialization phase (true/false")
                        .setNecessary(false)
                        .setValidator(std::regex("^(true|false)$"))
                        .buildAndReset()
                    )
                .buildAndReset()
            )
        },
        {"confirm", ParserCommandInfo(
            commandBuilder
                .setCallback(TypesHelper::methodToFunction(&MenuGameState::handleConfirm, this))
                .setDescription("Confirm the initialization phase and start the game")
                .setDisplayError(DefaultParserError::WrongFlagValueError)
                .buildAndReset()
            )
        }
    };
}

void MenuGameState::openState() {
    StateMessages::displayGreetingMessage("Menu");
}

void MenuGameState::closeState() {
    StateMessages::displayCloseMessage("Menu");
}

void MenuGameState::updateState() {
    std::string input;
    StateMessages::awaitCommandMessage();
    std::getline(std::cin, input);
    Parser parser(this->inputScheme, DefaultParserError::CommandNotFoundError);
    parser.executedParse(input);
}

GameState* MenuGameState::transitToState() {
    if (isRunning) {
        context.initialGameSubState = matchBuilder->getStateBuilder()();
        return new OngoingGameState(context);
    }

    return nullptr;
}
