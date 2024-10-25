#include "GameController.h"

#include "game-states/MenuGameState.h"
#include "view/game-states/GameView.h"
#include "../library/ViewHelper.h"

#include "library/TypesHelper.h"

GameController::GameController() {
    stateContext = StateContext();
    currentState = nullptr;
    currentMatch = nullptr;
}

GameController::~GameController() {
    delete currentState;
    delete currentMatch;
}

void GameController::finishGame(StateContext& context) {
    this->isFinished = true;
    int currentRound = context.currentMatch ? context.currentMatch->roundNumber: 0;
    ViewHelper::consoleOut("finishing game on round: " + std::to_string(currentRound));
}

void GameController::run() {
    stateContext.finishCallback = TypesHelper::methodToFunction(&GameController::finishGame, this);
    currentState = new MenuGameState(stateContext);
    currentState->openState();

    while (!this->isFinished) {
        GameState *newState = currentState->transitToState();

        if (newState) {
            currentState->closeState();
            currentState = newState;
            currentState->openState();
        }

        currentState->updateState();
    }
}
