#include "MenuGameState.h"

#include <iostream>
#include <cpp_warships/utilities/include/StateMessages.h>
#include <cpp_warships/utilities/include/ViewHelper.h>
#include <cpp_warships/utilities/include/TypesHelper.h>
#include <cpp_warships/input_parser/include/builder/ConfigCommandBuilder.h>
#include <cpp_warships/input_parser/include/DefaultParserError.h>
#include <cpp_warships/input_parser/include/VoidParser.h>
#include <cpp_warships/input_parser/include/builder/DefaultParameterBuilder.h>

#include "GameState.h"
#include "OngoingGameState.h"
#include "../save/GameSaveCreator.h"

void MenuGameState::handleGameLoad(cpp_warships::input_parser::ParsedOptions options) {
    std::string filename = options["filename"];

    if (this->matchBuilder->loadSave(filename)) {
        ViewHelper::consoleOut("Successfully read file from " + filename);
    }
}

void MenuGameState::handleNewGame(cpp_warships::input_parser::ParsedOptions options) {
    bool isDefault = options["default"] == "true";
    this->matchBuilder->newGame(isDefault);
    ViewHelper::consoleOut("Successfully initialized new game (to confirm use 'confirm' command)");
}

void MenuGameState::handleConfirm(cpp_warships::input_parser::ParsedOptions options) {
    isRunning = true;
}

void MenuGameState::handleInfo(cpp_warships::input_parser::ParsedOptions options) {
    this->matchBuilder->printBattleScreenshot();
}

void MenuGameState::handleList(cpp_warships::input_parser::ParsedOptions options) {
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
    cpp_warships::input_parser::ConfigCommandBuilder<void> commandBuilder;
    cpp_warships::input_parser::DefaultParameterBuilder parameterBuilder;

    inputScheme = {
        {"load", cpp_warships::input_parser::ParserCommandInfo(
            commandBuilder
                .setCallback(TypesHelper::methodToFunction(&MenuGameState::handleGameLoad, this))
                .setDescription("Load a game from a file")
                .setDisplayError(cpp_warships::input_parser::DefaultParserError::WrongFlagValueError)
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
        {"new", cpp_warships::input_parser::ParserCommandInfo(
            commandBuilder
                .setCallback(TypesHelper::methodToFunction(&MenuGameState::handleNewGame, this))
                .setDescription("Start new game from scratch")
                .setDisplayError(cpp_warships::input_parser::DefaultParserError::WrongFlagValueError)
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
        {"info", cpp_warships::input_parser::ParserCommandInfo(
            commandBuilder
                .setCallback(TypesHelper::methodToFunction(&MenuGameState::handleInfo, this))
                .setDescription("Print the latest screenshot from currently handled save")
                .setDisplayError(cpp_warships::input_parser::DefaultParserError::WrongFlagValueError)
                .buildAndReset()
            )
        },
        {"list", cpp_warships::input_parser::ParserCommandInfo(
            commandBuilder
                .setCallback(TypesHelper::methodToFunction(&MenuGameState::handleList, this))
                .setDescription("List all available saves")
                .setDisplayError(cpp_warships::input_parser::DefaultParserError::WrongFlagValueError)
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
        {"confirm", cpp_warships::input_parser::ParserCommandInfo(
            commandBuilder
                .setCallback(TypesHelper::methodToFunction(&MenuGameState::handleConfirm, this))
                .setDescription("Confirm the initialization phase and start the game")
                .setDisplayError(cpp_warships::input_parser::DefaultParserError::WrongFlagValueError)
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
    cpp_warships::input_parser::VoidParser parser(this->inputScheme, cpp_warships::input_parser::DefaultParserError::CommandNotFoundError);
    parser.executedParse(input);
}

GameState* MenuGameState::transitToState() {
    if (isRunning) {
        context.initialGameSubState = matchBuilder->getStateBuilder()();
        return new OngoingGameState(context);
    }

    return nullptr;
}
