#include "MenuGameState.h"

#include <iostream>

#include "GameState.h"
#include "OngoingGameState.h"
#include "ShutdownGameState.h"
#include "library/TypesHelper.h"
#include "library/parser-builder/ConfigCommandBuilder.h"
#include "library/defaults/DefaultParserError.h"
#include "../../library/ViewHelper.h"

void MenuGameState::handleStart(ParsedOptions options) {
    this->latestCommand = "start";
}

void MenuGameState::handleExit(ParsedOptions options) {
    this->latestCommand = "exit";
}

MenuGameState::MenuGameState(StateContext& context) : GameState(context) {
    this->menuView = new GameMenuView();

    ConfigCommandBuilder commandBuilder;

    this->inputScheme = {
        {"new", ParserCommandInfo(
            commandBuilder
                .setCallback(TypesHelper::methodToFunction(&MenuGameState::handleStart, this))
                .setDescription("The purpose of this function is to start the match")
                .setDisplayError(DefaultParserError::WrongFlagValueError)
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
        }
    };
}

MenuGameState::~MenuGameState() {
    delete this->menuView;
}

void MenuGameState::openState() {
    ViewHelper::consoleOut("Game Menu");
    ViewHelper::consoleOut("Start\t Save\t Exit\t\n");
}

void MenuGameState::closeState() {
    ViewHelper::consoleOut("Quit the menu");
}

void MenuGameState::updateState() {
    menuView->drawAvailableCommands(context.currentMatch);
    std::string input;
    std::getline(std::cin, input);
    Parser parser(this->inputScheme, DefaultParserError::CommandNotFoundError);
    parser.executedParse(input);
}

GameState* MenuGameState::transitToState() {
    if (latestCommand == "start"){
        return new OngoingGameState(context);
    }
    if (latestCommand == "exit") {
        return new ShutdownGameState(context);
    }

    return nullptr;
}
