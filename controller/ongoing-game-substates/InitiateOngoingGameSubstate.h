#pragma once
#include "game-states/OngoingGameState.h"
#include "game-states/GameState.h"
#include "PlaceShipController.h"

class InitiateOngoingGameSubstate : public OngoingGameState {
private:
    SchemeMap inputScheme;
    PlaceShipController* placeShipController;
    void handleConfirm(ParsedOptions options);
public:
    InitiateOngoingGameSubstate(StateContext& context);
    ~InitiateOngoingGameSubstate() override;
    void openSubstate() override;
    void updateSubstate() override;
    void closeSubstate() override;
    OngoingGameState* transitToSubstate() override;
};