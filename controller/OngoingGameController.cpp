#include "OngoingGameController.h"
#include "ongoing-game-substates/InitiateOngoingGameSubstate.h"
#include "model/StateContext.h"
#include "library/ViewHelper.h"
#include "library/TypesHelper.h"

OngoingGameController::OngoingGameController(StateContext& context) 
    : stateContext(context)
{   
    currentSubState = nullptr;
}

OngoingGameController::~OngoingGameController() {
    delete stateContext.currentMatch;
    delete currentSubState;
}

void OngoingGameController::finishMatch(StateContext& context) {
    this->isFinishedMatch = true;
    int currentRound = context.currentMatch ? context.currentMatch->roundNumber: 0;
    ViewHelper::consoleOut("finishing game on round: " + std::to_string(currentRound));
}

void OngoingGameController::run() {
    stateContext.finishMatchCallback = TypesHelper::methodToFunction(&OngoingGameController::finishMatch, this);
    currentSubState = new InitiateOngoingGameSubstate(stateContext);
    currentSubState->openSubstate();
    while (!this->isFinishedMatch) {
        OngoingGameState* newSubState = currentSubState->transitToSubstate();
        if (newSubState) {
            currentSubState->closeSubstate();
            currentSubState = newSubState;
            currentSubState->openSubstate();
        }

        currentSubState->updateSubstate();
    }
}