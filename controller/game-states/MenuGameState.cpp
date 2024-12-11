#include "MenuGameState.h"

#include <iostream>

#include "DefaultParameterBuilder.h"
#include "GameState.h"
#include "OngoingGameState.h"
#include "ShutdownGameState.h"
#include "StateMessages.h"
#include "ViewHelper.h"
#include "library/TypesHelper.h"
#include "library/parser-builder/ConfigCommandBuilder.h"
#include "library/defaults/DefaultParserError.h"

void MenuGameState::handleStart(ParsedOptions options) {
    context.loadFileName = options["filename"];
    this->latestCommand = "start";
}

void MenuGameState::handleExit(ParsedOptions options) {
    this->latestCommand = "exit";
}

MenuGameState::MenuGameState(StateContext& context) 
    : GameState(context)
{
    ConfigCommandBuilder commandBuilder;
    DefaultParameterBuilder parameterBuilder;
    this->inputScheme = {
        {"start", ParserCommandInfo(
            commandBuilder
                .setCallback(TypesHelper::methodToFunction(&MenuGameState::handleStart, this))
                .setDescription("The purpose of this function is to start the match")
                .setDisplayError(DefaultParserError::WrongFlagValueError)
                .addParameter(
                    parameterBuilder
                        .addFlag("--filename")
                        .setDescription("Write the name of the player")
                        .setValidator(std::regex("^.*\\.json$"))
                        .setNecessary(false)
                        .buildAndReset()
                )
                .buildAndReset()
            )
        },
        {"exit", ParserCommandInfo(
            commandBuilder
                .setCallback(TypesHelper::methodToFunction(&MenuGameState::handleExit, this))
                .setDescription("Leave the game")
                .setDisplayError(DefaultParserError::WrongFlagValueError)
                .buildAndReset()
            )
        },
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
    if (latestCommand == "start"){
        return new OngoingGameState(context);
    }
    else if (latestCommand == "exit") {
        return new ShutdownGameState(context);
    }
    return nullptr;
}
