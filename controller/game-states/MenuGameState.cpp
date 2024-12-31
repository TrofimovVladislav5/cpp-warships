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
#include "controller/save/GameSaveCreator.h"
#include "void/VoidParser.h"


void MenuGameState::handleGameLoad(ParsedOptions options) {
    std::string filename = options["filename"];

    if (this->matchBuilder->loadSave(filename)) {
        ViewHelper::consoleOut("Successfully read file from " + filename);
    }
}

void MenuGameState::handleNewGame(ParsedOptions options) {
    bool isDefault = options["default"] == "true";
    this->matchBuilder->newGame(isDefault);
    ViewHelper::consoleOut("Successfully initialized new game (to confirm use 'confirm' command)");
}

void MenuGameState::handleConfirm(ParsedOptions options) {
    isRunning = true;
}

void MenuGameState::handleInfo(ParsedOptions options) {
    this->matchBuilder->printBattleScreenshot();
}

void MenuGameState::handleList(ParsedOptions options) {
    std::string path = options["filename"];
    GameSaveCreator creator;
    std::vector<std::string> saves = creator.listSaves(path.empty() ? "." : path);

    ViewHelper::consoleOut("Available saves:");
    for (const auto& save : saves) {
        ViewHelper::consoleOut(save, 1);
    }
}

MenuGameState::MenuGameState(StateContext& context) 
    : GameState(context)
    , matchBuilder(new MatchBuilder(context.getInputReader()))
    , isRunning(false)
{
    ConfigCommandBuilder<void> commandBuilder;
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
        {"info", ParserCommandInfo(
            commandBuilder
                .setCallback(TypesHelper::methodToFunction(&MenuGameState::handleInfo, this))
                .setDescription("Print the latest screenshot from currently handled save")
                .setDisplayError(DefaultParserError::WrongFlagValueError)
                .buildAndReset()
            )
        },
        {"list", ParserCommandInfo(
            commandBuilder
                .setCallback(TypesHelper::methodToFunction(&MenuGameState::handleList, this))
                .setDescription("List all available saves")
                .setDisplayError(DefaultParserError::WrongFlagValueError)
                .addParameter(
                    parameterBuilder
                        .addFlag("--filename")
                        .setNecessary(false)
                        .setDescription("Specify path to the directory with saves")
                        .setValidator(std::regex("^.*$"))
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
    StateMessages::awaitCommandMessage();
    std::string input = context.getInputReader()->readCommand();
    VoidParser parser(this->inputScheme, DefaultParserError::CommandNotFoundError);
    parser.executedParse(input);
}

GameState* MenuGameState::transitToState() {
    if (isRunning) {
        context.initialGameSubState = matchBuilder->getStateBuilder()();
        return new OngoingGameState(context);
    }

    return nullptr;
}
