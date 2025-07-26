#include "TransitGameState.h"

#include <cpp_warships/utilities/include/StateMessages.h>
#include <cpp_warships/utilities/include/TypesHelper.h>
#include <cpp_warships/input_parser/include/builder/ConfigCommandBuilder.h>
#include <cpp_warships/input_parser/include/builder/DefaultParameterBuilder.h>
#include <cpp_warships/input_parser/include/DefaultParserError.h>
#include <cpp_warships/input_parser/include/VoidParser.h>

#include "MenuGameState.h"
#include "ShutdownGameState.h"

void TransitGameState::handleExit(cpp_warships::input_parser::ParsedOptions options) {
    latestCommand = "exit";
}

void TransitGameState::handleMenu(cpp_warships::input_parser::ParsedOptions options) {
    latestCommand = "menu";
}

TransitGameState::TransitGameState(StateContext &context)
    : GameState(context)
{
    cpp_warships::input_parser::ConfigCommandBuilder<void> commandBuilder;
    cpp_warships::input_parser::DefaultParameterBuilder parameterBuilder;

    inputScheme = {
        {"exit", cpp_warships::input_parser::ParserCommandInfo(
            commandBuilder
                .setCallback(TypesHelper::methodToFunction(&TransitGameState::handleExit, this))
                .setDescription("Leave the game")
                .setDisplayError(cpp_warships::input_parser::DefaultParserError::WrongFlagValueError)
                .buildAndReset()
            )
        },
        {"menu", cpp_warships::input_parser::ParserCommandInfo(
            commandBuilder
                .setCallback(TypesHelper::methodToFunction(&TransitGameState::handleMenu, this))
                .setDescription("Return to main menu")
                .setDisplayError(cpp_warships::input_parser::DefaultParserError::WrongFlagValueError)
                .buildAndReset()
            )
        }
    };
}

void TransitGameState::openState() {
}

void TransitGameState::updateState() {
    StateMessages::awaitCommandMessage();
    std::string input = context.getInputReader()->readCommand();
    cpp_warships::input_parser::VoidParser parser(this->inputScheme, cpp_warships::input_parser::DefaultParserError::CommandNotFoundError);
    parser.executedParse(input);
}

void TransitGameState::closeState() {
}

GameState* TransitGameState::transitToState() {
    if (latestCommand == "exit") {
        return new ShutdownGameState(context);
    } else if (latestCommand == "menu") {
        return new MenuGameState(context);
    }

    return nullptr;
}
