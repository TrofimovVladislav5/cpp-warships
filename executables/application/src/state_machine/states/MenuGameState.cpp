#include "../../../include/state_machine/states/MenuGameState.h"

#include <iostream>
#include <cpp_warships/utilities/include/StateMessages.h>
#include <cpp_warships/utilities/include/ViewHelper.h>
#include <cpp_warships/utilities/include/TypesHelper.h>
#include <cpp_warships/input_parser/include/builder/ConfigCommandBuilder.h>
#include <cpp_warships/input_parser/include/DefaultParserError.h>
#include <cpp_warships/input_parser/include/VoidParser.h>
#include <cpp_warships/input_parser/include/builder/DefaultParameterBuilder.h>

#include "../../../include/GameSaveCreator.h"
#include "../../../include/state_machine/GameState.h"
#include "../../../include/state_machine/states/OngoingGameState.h"

namespace cpp_warships::application {

    void MenuGameState::handleGameLoad(input_parser::ParsedOptions options) {
        std::string filename = options["filename"];

        if (matchBuilder.loadSave(filename)) {
            ViewHelper::consoleOut("Successfully read file from " + filename);
        }
    }

    void MenuGameState::handleNewGame(input_parser::ParsedOptions options) {
        bool isDefault = options["default"] == "true";
        matchBuilder.newGame(isDefault);

        ViewHelper::consoleOut("Successfully initialized new game (to confirm use 'confirm' command)");
    }

    void MenuGameState::handleConfirm(input_parser::ParsedOptions options) {
        isRunning = true;
    }

    void MenuGameState::handleInfo(input_parser::ParsedOptions options) {
        matchBuilder.printBattleScreenshot();
    }

    void MenuGameState::handleList(input_parser::ParsedOptions options) {
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
        , matchBuilder({ context.getInputReader() })
        , isRunning(false)
    {
        input_parser::ConfigCommandBuilder<void> commandBuilder;
        input_parser::DefaultParameterBuilder parameterBuilder;

        inputScheme = {
            {"load", input_parser::ParserCommandInfo(
                commandBuilder
                    .setCallback(TypesHelper::methodToFunction(&MenuGameState::handleGameLoad, this))
                    .setDescription("Load a game from a file")
                    .setDisplayError(input_parser::DefaultParserError::WrongFlagValueError)
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
            {"new", input_parser::ParserCommandInfo(
                commandBuilder
                    .setCallback(TypesHelper::methodToFunction(&MenuGameState::handleNewGame, this))
                    .setDescription("Start new game from scratch")
                    .setDisplayError(input_parser::DefaultParserError::WrongFlagValueError)
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
            {"info", input_parser::ParserCommandInfo(
                commandBuilder
                    .setCallback(TypesHelper::methodToFunction(&MenuGameState::handleInfo, this))
                    .setDescription("Print the latest screenshot from currently handled save")
                    .setDisplayError(input_parser::DefaultParserError::WrongFlagValueError)
                    .buildAndReset()
                )
            },
            {"list", input_parser::ParserCommandInfo(
                commandBuilder
                    .setCallback(TypesHelper::methodToFunction(&MenuGameState::handleList, this))
                    .setDescription("List all available saves")
                    .setDisplayError(input_parser::DefaultParserError::WrongFlagValueError)
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
            {"confirm", input_parser::ParserCommandInfo(
                commandBuilder
                    .setCallback(TypesHelper::methodToFunction(&MenuGameState::handleConfirm, this))
                    .setDescription("Confirm the initialization phase and start the game")
                    .setDisplayError(input_parser::DefaultParserError::WrongFlagValueError)
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

        input_parser::VoidParser parser(inputScheme, input_parser::DefaultParserError::CommandNotFoundError);
        parser.executedParse(input);
    }

    GameState* MenuGameState::transitToState() {
        if (isRunning) {
            context.initialGameSubState = matchBuilder.getStateBuilder()();
            return new OngoingGameState(context);
        }

        return nullptr;
    }
} // namespace cpp_warships::application
