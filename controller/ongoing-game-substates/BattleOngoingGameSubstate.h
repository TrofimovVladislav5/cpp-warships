#pragma once
#include "model/StateContext.h"
#include "game-states/OngoingGameState.h"

class BattleOngoingGameSubstate : public OngoingGameState {
public:
    BattleOngoingGameSubstate(StateContext& context);
    ~BattleOngoingGameSubstate();
    void openSubstate() override;
    void closeSubstate() override;
    void updateSubstate() override;
    OngoingGameState* transitToSubstate() override;
};
