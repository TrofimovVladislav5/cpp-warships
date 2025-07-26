#pragma once

#include "OngoingGameSubState.h"
#include "../PlaceShipController.h"
#include "../BattleController.h"
#include "../../view/BattleView.h"

class BattleOngoingGameSubState : public OngoingGameSubState {
private:
    cpp_warships::input_parser::SchemeMap<void> inputScheme;
    BattleController* battleController;
    PlaceShipController* enemyPlaceController;
    BattleView view;
    bool isPaused;
    OngoingGameSubState* handleComputerWin() const;
    OngoingGameSubState* handlePlayerWin();
    void handlePause(cpp_warships::input_parser::ParsedOptions options);
public:
    explicit BattleOngoingGameSubState(SubStateContext* context);
    ~BattleOngoingGameSubState() override;
    void openSubState() override;
    void closeSubState() override;
    void updateSubState() override;
    OngoingGameSubState* transitToSubState() override;
};
