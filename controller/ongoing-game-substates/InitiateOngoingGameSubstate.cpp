#include <iostream>
#include "game-states/GameState.h"
#include "view/ViewHelper.h"
#include "model/StateContext.h"
#include "game-states/OngoingGameState.h"
#include "InitiateOngoingGameSubstate.h"
#include "BattleOngoingGameSubstate.h"

InitiateOngoingGameSubstate::InitiateOngoingGameSubstate(StateContext& context) : OngoingGameState(context){
    this->placeShipController = new PlaceShipController();
};   

InitiateOngoingGameSubstate::~InitiateOngoingGameSubstate(){
    delete placeShipController;
}

void InitiateOngoingGameSubstate::openSubstate() {
    ViewHelper::consoleOut("Welcome to placement ships");
    ViewHelper::consoleOut("Rules of placeShip length, direction, no crossing, about some maximal(fieldsize, countShips)");
}

void InitiateOngoingGameSubstate::updateSubstate() {
    ViewHelper::consoleOut("add, delete ships");
    std:::getline(std::cin, latestCommand);
    if (latestCommand == "add") {
        placeShipController->add();
    }
    else if (latestCommand == "delete") {
        placeShipController->delete();
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