#include <iostream>
#include "../../library/ViewHelper.h"
#include "InitiateOngoingGameSubState.h"
#include "../PlaceShipController.h"
#include "BattleOngoingGameSubState.h"
#include "ShipPlacementException.h"
#include "StateMessages.h"
#include "library/parser-builder/ConfigCommandBuilder.h"
#include "library/parser/ParserCommandInfo.h"
#include "library/defaults/DefaultParserError.h"
#include "library/TypesHelper.h"
#include "library/parser-builder/DefaultParameterBuilder.h"


InitiateOngoingGameSubState::InitiateOngoingGameSubState(SubStateContext& context)
    : OngoingGameSubState(context)
    , placeShipController(new PlaceShipController(*context.settings))
{
    ConfigCommandBuilder commandBuilder;
    DefaultParameterBuilder parameterBuilder;
    this->inputScheme = {
        {"add", ParserCommandInfo(
            commandBuilder
                .setCallback(TypesHelper::methodToFunction(&PlaceShipController::addShip, placeShipController))
                .setDescription("Adds a ship to the game field")
                .setDisplayError(DefaultParserError::WrongFlagValueError)
                .addParameter(
                    parameterBuilder
                        .addFlag("--length")
                        .setValidator(std::regex("^[1-4]$"))
                        .setNecessary(true)
                        .buildAndReset()
                )

                .addParameter (
                    parameterBuilder
                        .addFlag("--direction")
                        .setValidator(std::regex("^(horizontal|vertical)$"))
                        .setNecessary(true)
                        .buildAndReset()
                )

                .addParameter (
                    parameterBuilder
                        .addFlag("--cell")
                        .setValidator(std::regex("^(\\d+),(\\d+)$"))
                        .setNecessary(true)
                        .buildAndReset()
                )
                .buildAndReset()
        )},
        {"remove", ParserCommandInfo(
            commandBuilder
                .setCallback(TypesHelper::methodToFunction(&PlaceShipController::removeShip, placeShipController))
                .setDescription("Removes ship on field")
                .setDisplayError(DefaultParserError::WrongFlagValueError)
                .addParameter(
                    parameterBuilder
                        .addFlag("--cell")
                        .setValidator(std::regex("^(\\d+),(\\d+)$"))
                        .setNecessary(true)
                        .buildAndReset()
                )
                .buildAndReset()
        )}
    };
}

InitiateOngoingGameSubState::~InitiateOngoingGameSubState(){
    delete placeShipController;
}

void InitiateOngoingGameSubState::openSubState() {
    StateMessages::displayGreetingMessage("OngoingGame.PlaceShips");
}

void InitiateOngoingGameSubState::updateSubState() {
    StateMessages::awaitCommandMessage();

    Parser parser(this->inputScheme, DefaultParserError::CommandNotFoundError);
    try {
        std::string input;
        std::getline(std::cin, input);
        parser.executedParse(input);
    } catch (const ShipPlacementException& exception) {
        exception.displayError();
    }
}

void InitiateOngoingGameSubState::closeSubState() {
    StateMessages::displayCloseMessage("OngoingGame.Initiate");
}

OngoingGameSubState* InitiateOngoingGameSubState::transitToSubState() {
    if (placeShipController->allShipsPlaced()) {
        ViewHelper::consoleOut("Do you want to confirm these settings? (yes/no)");
        if (ViewHelper::confirmAction("yes")) {
            placeShipController->placeShipComputer();
            return new BattleOngoingGameSubState(context);
        }
    }

    return nullptr;
}