#include "GameController.h"

#include "game-states/MenuGameState.h"
#include "game-states/ShutdownGameState.h"
#include "view/GameView.h"
#include "view/ViewHelper.h"

#include <typeinfo>

GameController::GameController() {
    stateContext = StateContext();
    currentState = nullptr;
    view = new GameView();
    currentMatch = nullptr;
}

GameController::~GameController() {
    delete currentState;
    delete view;
    delete currentMatch;
}

void GameController::run() {
    currentState = new MenuGameState(stateContext);

    while (typeid(*currentState).name() != typeid(ShutdownGameState).name()) {
        GameState *newState = currentState->transitToState();

        if (newState) {
            currentState->closeState();
            currentState = newState;
            currentState->openState();
        }

        currentState->updateState();
    }
}
