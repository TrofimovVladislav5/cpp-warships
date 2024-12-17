#pragma once
#include "BattleView.h"
#include "OngoingGameSubState.h"
#include "PlaceShipController.h"
#include "../BattleController.h"

class BattleOngoingGameSubState : public OngoingGameSubState {
private:
    SchemeMap inputScheme;
    BattleController* battleController;
    PlaceShipController* enemyPlaceController;
    BattleView view;
    OngoingGameSubState* handleFinishBattle();
public:
    explicit BattleOngoingGameSubState(SubStateContext* context);
    ~BattleOngoingGameSubState() override;
    void openSubState() override;
    void closeSubState() override;
    void updateSubState() override;
    OngoingGameSubState* transitToSubState() override;
};
