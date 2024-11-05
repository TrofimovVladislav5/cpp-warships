#pragma once
#include "game-states/OngoingGameState.h"
class FinishOngoingGameSubstate : public OngoingGameState{
private:
    StateContext context;
public:
    explicit FinishOngoingGameSubstate(StateContext& context);
};
