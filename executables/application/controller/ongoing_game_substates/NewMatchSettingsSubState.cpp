#include "NewMatchSettingsSubState.h"

#include <cpp_warships/utilities/include/TypesHelper.h>
#include <cpp_warships/utilities/include/ViewHelper.h>
#include <cpp_warships/utilities/include/StateMessages.h>
#include <cpp_warships/input_parser/include/builder/ConfigCommandBuilder.h>
#include <cpp_warships/input_parser/include/builder/DefaultParameterBuilder.h>
#include <cpp_warships/input_parser/include/DefaultParserError.h>
#include <cpp_warships/input_parser/include/VoidParser.h>

#include "InitiateOngoingGameSubState.h"
#include "../ShipManager.h"

namespace cpp_warships::application {
    void NewMatchSettingsSubState::handleMatchSettings(input_parser::ParsedOptions options) {
        int fieldSize = std::stoi(options["size"]);
        currentSettings = controller->createMatchSettings(fieldSize);
    }

    NewMatchSettingsSubState::NewMatchSettingsSubState(SubStateContext* context)
        : OngoingGameSubState(context)
        , controller(new MatchSettingsController())
        , currentSettings(nullptr)
    {
        input_parser::ConfigCommandBuilder<void> commandBuilder;
        input_parser::DefaultParameterBuilder parameterBuilder;
        this->inputScheme = {
            {"set", input_parser::ParserCommandInfo(
                commandBuilder
                    .setCallback(TypesHelper::methodToFunction(&NewMatchSettingsSubState::handleMatchSettings, this))
                    .setDescription("Choose a field size between 10 and 25")
                    .setDisplayError(input_parser::DefaultParserError::WrongFlagValueError)
                    .addParameter(
                        parameterBuilder
                            .addFlag("--size")
                            .setValidator(std::regex("^(1[0-9]|2[0-5])$"))
                            .setNecessary(true)
                            .buildAndReset()
                    )
                    .buildAndReset()
            )}
        };
    }

    NewMatchSettingsSubState::~NewMatchSettingsSubState() {
        delete currentSettings;
        delete controller;
    }

    void NewMatchSettingsSubState::openSubState() {
    }

    void NewMatchSettingsSubState::closeSubState() {
    }

    void NewMatchSettingsSubState::updateSubState() {
        StateMessages::awaitCommandMessage();
        std::string input = context->getInputReader()->readCommand();
        input_parser::VoidParser parser(this->inputScheme, input_parser::DefaultParserError::CommandNotFoundError);
        parser.executedParse(input);
    }

    OngoingGameSubState* NewMatchSettingsSubState::transitToSubState() {
        if (currentSettings) {
            auto* playerManager = new ShipManager(currentSettings->getShipsCount());
            std::vector<Ship*> ships = playerManager->getShips();
            std::map<int, int> shipsSizes = currentSettings->getShipsCount();

            ViewHelper::consoleOut("Optimal set of ships: ");
            for (const auto& [size, amount] : shipsSizes) {
                std::string sizeString = std::string("Amount of ships with size ")
                    .append(std::to_string(size))
                    .append(": ")
                    .append(std::to_string(amount));

                ViewHelper::consoleOut(sizeString, 1);
            }

            if (ViewHelper::confirmAction(
                TypesHelper::methodToFunction(&input_reader::InputReader<>::readCommand, context->getInputReader()),
                "yes"
            )) {
                this->context->matchDTO = new GameStateDTO(currentSettings);
                return new InitiateOngoingGameSubState(context);
            } else {
                this->currentSettings = nullptr;
            }
        }

        return nullptr;
    }
} // namespace cpp_warships::application