#pragma once
#include "model/StateContext.h"
#include "ongoing-game-substates/BattleController.h"
#include "game-states/OngoingGameState.h"

class BattleOngoingGameSubstate : public OngoingGameState {
private:
    SchemeMap inputScheme;
    StateContext context;
    BattleController* battleController;
public:
    explicit BattleOngoingGameSubstate(StateContext& context);
    ~BattleOngoingGameSubstate() override;
    void openSubstate() override;
    void closeSubstate() override;
    void updateSubstate() override;
    OngoingGameState* transitToSubstate() override;
};
