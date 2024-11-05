#include <iostream>
#include "game-states/GameState.h"
#include "../../library/ViewHelper.h"
#include "model/StateContext.h"
#include "game-states/OngoingGameState.h"
#include "InitiateOngoingGameSubstate.h"
#include "PlaceShipController.h"
#include "BattleOngoingGameSubstate.h"
#include "ShipPlacementException.h"
#include "library/parser-builder/ConfigCommandBuilder.h"
#include "library/parser/ParserCommandInfo.h"
#include "library/defaults/DefaultParserError.h"
#include "library/TypesHelper.h"
#include "library/parser-builder/DefaultParameterBuilder.h"
#include "view/game-substates/InitiateOngoingGameView.h"

InitiateOngoingGameSubstate::InitiateOngoingGameSubstate(StateContext& context)
    : OngoingGameState(context)
{   
    this->placeShipController = new PlaceShipController(context);
    this->ongoingGameView = new InitiateOngoingGameView();
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
        )},
        {"confirm", ParserCommandInfo(
                commandBuilder
                .setCallback(TypesHelper::methodToFunction(&InitiateOngoingGameSubstate::handleConfirm, this))
                .setDescription("Confirmation of the placement of ships")
                .setDisplayError(DefaultParserError::CommandNotFoundError)
                .buildAndReset()
        )},
    };
}

void InitiateOngoingGameSubstate::handleConfirm(ParsedOptions options) {
    this->latestCommand = "confirm";
}

InitiateOngoingGameSubstate::~InitiateOngoingGameSubstate(){
    delete placeShipController;
}

void InitiateOngoingGameSubstate::openSubstate() {
    ongoingGameView->displayOpenState();
}

void InitiateOngoingGameSubstate::updateSubstate() {
    ongoingGameView->displayAvailableCommands(context.currentMatch);
    Parser parser(this->inputScheme, DefaultParserError::CommandNotFoundError);
    std::string input;
    std::getline(std::cin, input);
    latestCommand = input;
    try {
        parser.executedParse(input);
    }
    catch (const ShipPlacementException& exception) {
        exception.displayError();
    }
}

void InitiateOngoingGameSubstate::closeSubstate() {
    ongoingGameView->displayCloseState();
}

OngoingGameState* InitiateOngoingGameSubstate::transitToSubstate() {
    if (latestCommand == "confirm" && placeShipController->allShipsPlaced()) {
        placeShipController->placeShipComputer();
        return new BattleOngoingGameSubstate(context);
    }
    return nullptr;
}