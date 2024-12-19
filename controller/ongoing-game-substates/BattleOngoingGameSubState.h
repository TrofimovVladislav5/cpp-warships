#pragma once
#include "OngoingGameSubState.h"
#include "../BattleController.h"
#include "game-states/OngoingGameState.h"


class BattleOngoingGameSubState : public OngoingGameSubState {
private:
    SchemeMap inputScheme;
    BattleController* battleController;
public:
    explicit BattleOngoingGameSubState(SubStateContext& context);
    ~BattleOngoingGameSubState() override;
    void openSubState() override;
    void closeSubState() override;
    void updateSubState() override;
    OngoingGameSubState* transitToSubState() override;
};
