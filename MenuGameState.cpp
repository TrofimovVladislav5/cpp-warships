#include <iostream>
#include "GameState.h"
#include "MenuGameState.h"
#include "OngoingGameState.h"
#include "GameView.h"

void MenuGameState::openState() {
    view->displayMessage("Game Menu");
    view->displayMessage("Start\t Save\t Exit\t");
}

void MenuGameState::closeState() {
    view->displayMessage("Quit the menu");
}

void MenuGameState::updateState() {
    std::getline(std::cin,latestCommand);
}

GameState* MenuGameState::transitToState() {
    if (latestCommand == "start"){
        return new OngoingGameState();
    }
    return nullptr;
        // else if (latestCommand == "exit"){
        //     newState = new ExitGamseState();
        //     return true;
        // }
        // else if (latestCommand == "save"){
        //     newState = new SaveGameState();
        //     return true;
        // }
        // return false;
}
