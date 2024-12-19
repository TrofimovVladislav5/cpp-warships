#pragma once

#include "GameState.h"
#include "model/StateContext.h"
#include "controller/ongoing-game-substates/OngoingGameSubState.h"

class OngoingGameState : public GameState {
protected:
    OngoingGameSubState* currentSubState;

public:
    OngoingGameState(StateContext &context);
    void openState() override;
    void updateState() override;
    void closeState() override;
    GameState* transitToState() override;
};
