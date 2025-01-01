#pragma once

#include "GameState.h"
#include "src/model/StateContext.h"
#include "src/controller/ongoing-game-substates/OngoingGameSubState.h"

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
