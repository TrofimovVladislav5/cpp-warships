#include "PauseOngoingGameSubState.h"

#include <iostream>
#include <cpp_warships/utilities/include/StateMessages.h>
#include <cpp_warships/utilities/include/TypesHelper.h>
#include <cpp_warships/utilities/include/ViewHelper.h>
#include <cpp_warships/input_parser/include/VoidParser.h>
#include <cpp_warships/input_parser/include/DefaultParserError.h>
#include <cpp_warships/input_parser/include/builder/DefaultParameterBuilder.h>
#include <cpp_warships/input_parser/include/builder/ConfigCommandBuilder.h>

#include "BattleOngoingGameSubState.h"

void PauseOngoingGameSubState::handleResume(cpp_warships::input_parser::ParsedOptions options) {
    try {
        loadedSubState = new BattleOngoingGameSubState(context);
    } catch (std::exception &e) {
        ViewHelper::errorOut("Something went wrong while resuming the game", e);
        loadedSubState = nullptr;
    }
}

void PauseOngoingGameSubState::handleLoad(cpp_warships::input_parser::ParsedOptions options) {
    try {
        matchBuilder.loadSave(options["filename"]);
        ViewHelper::consoleOut("Save loaded successfully");

        const bool isForcedLoad = options["force"] == "true";
        const bool isConfirmed = isForcedLoad || ViewHelper::confirmAction(
            TypesHelper::methodToFunction(&cpp_warships::input_reader::InputReader<>::readCommand, context->getInputReader()),
            "yes"
        );

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

void PauseOngoingGameSubState::handleSave(cpp_warships::input_parser::ParsedOptions options) {
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
    , matchBuilder(MatchBuilder(context->getInputReader()))
    , view(GamePauseView(30))
    , loadedSubState(nullptr)
{
    cpp_warships::input_parser::ConfigCommandBuilder<void> commandBuilder;
    cpp_warships::input_parser::DefaultParameterBuilder parameterBuilder;
    this->inputScheme = {
        {"resume", cpp_warships::input_parser::ParserCommandInfo(
            commandBuilder
                .setCallback(TypesHelper::methodToFunction(&PauseOngoingGameSubState::handleResume, this))
                .setDescription("Continue the battle")
                .setDisplayError(cpp_warships::input_parser::DefaultParserError::CommandNotFoundError)
                .buildAndReset()
        )},
        {"save", cpp_warships::input_parser::ParserCommandInfo(
                commandBuilder
                .setCallback(TypesHelper::methodToFunction(&PauseOngoingGameSubState::handleSave, this))
                .setDescription("Save the game")
                .setDisplayError(cpp_warships::input_parser::DefaultParserError::CommandNotFoundError)
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
        {"load", cpp_warships::input_parser::ParserCommandInfo(
                commandBuilder
                .setCallback(TypesHelper::methodToFunction(&PauseOngoingGameSubState::handleLoad, this))
                .setDescription("Load the game")
                .setDisplayError(cpp_warships::input_parser::DefaultParserError::CommandNotFoundError)
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
        cpp_warships::input_parser::VoidParser parser(this->inputScheme, cpp_warships::input_parser::DefaultParserError::CommandNotFoundError);
        std::string input = context->getInputReader()->readCommand();
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