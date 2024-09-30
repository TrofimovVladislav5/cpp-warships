#pragma once
#include "game-states/OngoingGameState.h"
#include "game-states/GameState.h"

class PauseOngoingGameSubstate : public OngoingGameState {
public:
    void openSubstate() override;
    void closeSubstate() override;
    void updateSubstate() override;
    OngoingGameState* transitToSubstate() override;
};
