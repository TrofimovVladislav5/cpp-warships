#include <iostream>
#include "GameState.h"
#include "OngoingGameState.h"
#include "GameView.h"

void OngoingGameState::openState() {
    view->displayMessage("Ongoing game");
}
void OngoingGameState::closeState() {
    view->displayMessage("Quit the game");
}

void OngoingGameState::updateState() {
    std::getline(std::cin, latestCommand);
}

GameState* OngoingGameState::transitToState() {
    if (latestCommand == "start"){
        return new OngoingGameState();
    }
    else if (latestCommand == "menu"){
        return new MenuGameState();
    }
    return nullptr;
}
