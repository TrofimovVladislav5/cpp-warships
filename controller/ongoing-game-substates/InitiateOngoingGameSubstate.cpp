#include <iostream>
#include "game-states/GameState.h"
#include "view/ViewHelper.h"
#include "model/StateContext.h"
#include "game-states/OngoingGameState.h"
#include "InitiateOngoingGameSubstate.h"
#include "PlaceShipController.h"
#include "BattleOngoingGameSubstate.h"

InitiateOngoingGameSubstate::InitiateOngoingGameSubstate(StateContext& context) : OngoingGameState(context){
    this->placeShipController = new PlaceShipController(context);
};   

InitiateOngoingGameSubstate::~InitiateOngoingGameSubstate(){
    delete placeShipController;
}

void InitiateOngoingGameSubstate::openSubstate() {
    ViewHelper::consoleOut("Welcome to placement ships");
    ViewHelper::consoleOut("Rules of placeShip length, direction, no crossing, about some maximal(fieldsize, countShips)");
}

void InitiateOngoingGameSubstate::updateSubstate() {
    ViewHelper::consoleOut("You can in this stage add and delete ships on game field");
    std::getline(std::cin, latestCommand);
    if (latestCommand == "add") {
        placeShipController->addShip();
    }
    else if (latestCommand == "delete") {
        placeShipController->deleteShip();
    }
}

void InitiateOngoingGameSubstate::closeSubstate() {
    ViewHelper::consoleOut("The placement of ships is completed");
}

OngoingGameState* InitiateOngoingGameSubstate::transitToSubstate() {
    if (latestCommand == "attack") {
        return new BattleOngoingGameSubstate(context);
    }
    return nullptr;
}