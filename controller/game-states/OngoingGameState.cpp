#include <iostream>
#include "GameState.h"
#include "OngoingGameState.h"

#include "MenuGameState.h"
#include "view/ViewHelper.h"

OngoingGameState::OngoingGameState(StateContext &context) : GameState(context) {
}

void OngoingGameState::openState() {
    //TODO: Implement printing out the 'hello' letter (game rules, etc.)
    ViewHelper::consoleOut("Ongoing game");
}

void OngoingGameState::updateState() {
    //TODO: Same logic as in GameController

    // currentState = new MenuGameState(stateContext);
    //
    // while (typeid(*currentState).name() != typeid(ShutdownGameState).name()) {
    //     GameState *newState = currentState->transitToState();
    //
    //     if (newState) {
    //         currentState->closeState();
    //         currentState = newState;
    //         currentState->openState();
    //     }
    //
    //     currentState->updateState();
    // }
}

void OngoingGameState::closeState() {
    ViewHelper::consoleOut("Quit the game");
}

GameState* OngoingGameState::transitToState() {
    if (latestCommand == "start"){
        return new OngoingGameState(context);
    }
    else if (latestCommand == "menu"){
        return new MenuGameState(context);
    }
    return nullptr;
}
