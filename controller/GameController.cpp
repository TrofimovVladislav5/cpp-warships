#include "GameController.h"

#include "ViewHelper.h"
#include "game-states/MenuGameState.h"


GameController::GameController()
    : stateContext(StateContext())
    , currentState(nullptr)
    , currentMatchData(nullptr)
{}

GameController::~GameController() {
    delete currentState;
    delete currentMatchData;
}

void GameController::run() {
    currentState = new MenuGameState(stateContext);
    currentState->openState();
    currentState->updateState();

    while (
        !stateContext.currentMatchData ||
        (stateContext.currentMatchData && !stateContext.currentMatchData->isFinished)
    ) {
        GameState *newState = currentState->transitToState();

        if (newState) {
            currentState->closeState();
            currentState = newState;
            currentState->openState();
        }

        currentState->updateState();
    }
}