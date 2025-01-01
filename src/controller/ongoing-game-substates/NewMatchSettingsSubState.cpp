#include "NewMatchSettingsSubState.h"
#include "src/library/parser-builder/ConfigCommandBuilder.h"
#include "src/library/parser-builder/DefaultParameterBuilder.h"
#include "src/library/TypesHelper.h"
#include "src/library/defaults/DefaultParserError.h"
#include "InitiateOngoingGameSubState.h"
#include "../ShipManager.h"
#include "StateMessages.h"
#include "ViewHelper.h"
#include "void/VoidParser.h"


void NewMatchSettingsSubState::handleMatchSettings(ParsedOptions options) {
    int fieldSize = std::stoi(options["size"]);
    currentSettings = controller->createMatchSettings(fieldSize);
}

NewMatchSettingsSubState::NewMatchSettingsSubState(SubStateContext* context)
    : OngoingGameSubState(context)
    , controller(new MatchSettingsController())
    , currentSettings(nullptr)
{
    ConfigCommandBuilder<void> commandBuilder;
    DefaultParameterBuilder parameterBuilder;
    this->inputScheme = {
        {"set", ParserCommandInfo(
            commandBuilder
                .setCallback(TypesHelper::methodToFunction(&NewMatchSettingsSubState::handleMatchSettings, this))
                .setDescription("Choose a field size between 10 and 25")
                .setDisplayError(DefaultParserError::WrongFlagValueError)
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
    VoidParser parser(this->inputScheme, DefaultParserError::CommandNotFoundError);
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

        if (ViewHelper::confirmAction(context->getInputReader(), "yes")) {
            this->context->matchDTO = new GameStateDTO(currentSettings);
            return new InitiateOngoingGameSubState(context);
        } else {
            this->currentSettings = nullptr;
        }
    }

    return nullptr;
}