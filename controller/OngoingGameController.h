#pragma once
#include "model/StateContext.h"
#include "game-states/OngoingGameState.h"

class OngoingGameController {
private:
    StateContext stateContext;
    OngoingGameState* currentSubState;
    void finishMatch(StateContext& context);
    bool isFinishedMatch = false;
public:
    OngoingGameController(StateContext& context);
    ~OngoingGameController();
    void run();
};