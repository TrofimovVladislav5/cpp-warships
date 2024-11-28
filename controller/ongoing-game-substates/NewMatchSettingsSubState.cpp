#include "NewMatchSettingsSubState.h"
#include "library/parser-builder/ConfigCommandBuilder.h"
#include "library/parser-builder/DefaultParameterBuilder.h"
#include "library/TypesHelper.h"
#include "library/defaults/DefaultParserError.h"
#include "InitiateOngoingGameSubState.h"
#include "ShipManager.h"
#include "StateMessages.h"
#include "ViewHelper.h"


void NewMatchSettingsSubState::handleMatchSettings(ParsedOptions options) {
    int fieldSize = std::stoi(options["size"]);
    currentSettings = controller->createMatchSettings(fieldSize);
}

NewMatchSettingsSubState::NewMatchSettingsSubState(SubStateContext& context)
    : OngoingGameSubState(context)
    , controller(new MatchSettingsController())
    , currentSettings(nullptr)
{
    ConfigCommandBuilder commandBuilder;
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
                        .setNecessary(false)
                        .buildAndReset()
                )
                .buildAndReset()

        )}
    };
}

NewMatchSettingsSubState::~NewMatchSettingsSubState() {
    delete controller;
}

void NewMatchSettingsSubState::openSubState() {
}

void NewMatchSettingsSubState::closeSubState() {
}

void NewMatchSettingsSubState::updateSubState() {
    StateMessages::awaitCommandMessage();

    std::string input;
    std::getline(std::cin, input);
    Parser parser(this->inputScheme, DefaultParserError::CommandNotFoundError);
    parser.executedParse(input);
}

OngoingGameSubState* NewMatchSettingsSubState::transitToSubState() {
    if (!currentSettings) {
        return nullptr;
    } else {
        auto* playerManager = new ShipManager(currentSettings->getShipsCount());
        std::vector<Ship*> ships = playerManager->getShips();
        ViewHelper::consoleOut("Optimal set of ships is: ");
        for (const auto& ship : ships) {
            ViewHelper::consoleOut(std::to_string(ship->getLength()), 1);
        }

        ViewHelper::consoleOut("Do you want to confirm these settings? (yes/no)");
        if (ViewHelper::confirmAction("yes")) {
            this->context.matchDTO = new GameStateDTO(currentSettings);
            return new InitiateOngoingGameSubState(context);
        }
    }

    return nullptr;
}