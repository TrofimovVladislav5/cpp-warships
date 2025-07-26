#include "TransitGameState.h"

#include <cpp_warships/utilities/include/StateMessages.h>
#include <cpp_warships/utilities/include/TypesHelper.h>
#include <cpp_warships/input_parser/include/builder/ConfigCommandBuilder.h>
#include <cpp_warships/input_parser/include/builder/DefaultParameterBuilder.h>
#include <cpp_warships/input_parser/include/DefaultParserError.h>
#include <cpp_warships/input_parser/include/VoidParser.h>

#include "MenuGameState.h"
#include "ShutdownGameState.h"

namespace cpp_warships::application {

    void TransitGameState::handleExit(input_parser::ParsedOptions options) {
        latestCommand = "exit";
    }

    void TransitGameState::handleMenu(input_parser::ParsedOptions options) {
        latestCommand = "menu";
    }

    TransitGameState::TransitGameState(StateContext &context)
        : GameState(context)
    {
        input_parser::ConfigCommandBuilder<void> commandBuilder;
        input_parser::DefaultParameterBuilder parameterBuilder;

        inputScheme = {
            {"exit", input_parser::ParserCommandInfo(
                commandBuilder
                    .setCallback(TypesHelper::methodToFunction(&TransitGameState::handleExit, this))
                    .setDescription("Leave the game")
                    .setDisplayError(input_parser::DefaultParserError::WrongFlagValueError)
                    .buildAndReset()
                )
            },
            {"menu", input_parser::ParserCommandInfo(
                commandBuilder
                    .setCallback(TypesHelper::methodToFunction(&TransitGameState::handleMenu, this))
                    .setDescription("Return to main menu")
                    .setDisplayError(input_parser::DefaultParserError::WrongFlagValueError)
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
        input_parser::VoidParser parser(this->inputScheme, input_parser::DefaultParserError::CommandNotFoundError);
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
} // namespace cpp_warships::application
