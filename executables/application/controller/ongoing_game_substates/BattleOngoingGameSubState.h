#pragma once

#include "OngoingGameSubState.h"
#include "../PlaceShipController.h"
#include "../BattleController.h"
#include "../../view/BattleView.h"

namespace cpp_warships::application {

    class BattleOngoingGameSubState : public OngoingGameSubState {
    private:
        input_parser::SchemeMap<void> inputScheme;
        BattleController* battleController;
        PlaceShipController* enemyPlaceController;
        BattleView view;
        bool isPaused;
        OngoingGameSubState* handleComputerWin() const;
        OngoingGameSubState* handlePlayerWin();
        void handlePause(input_parser::ParsedOptions options);
    public:
        explicit BattleOngoingGameSubState(SubStateContext* context);
        ~BattleOngoingGameSubState() override;
        void openSubState() override;
        void closeSubState() override;
        void updateSubState() override;
        OngoingGameSubState* transitToSubState() override;
    };
} // namespace cpp_warships::application
