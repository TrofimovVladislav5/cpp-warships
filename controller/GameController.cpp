#include "GameController.h"
#include "game-states/MenuGameState.h"
#include "view/GameView.h"

GameController::GameController() {
    currentState = new MenuGameState();
    view = new GameView();
}

GameController::~GameController() {
    delete currentState;
    delete view;
}

void GameController::run() {
    while (true) {
        GameState *newState = currentState->transitToState();
        if (newState) {
            currentState->closeState();
            currentState = newState;
            currentState->openState();
        }
        currentState->updateState();
    }
}
