#pragma once
#include "game-states/OngoingGameState.h"
#include "game-states/GameState.h"
#include "PlaceShipController.h"

class InitiateOngoingGameSubstate : public OngoingGameState {
private:
    PlaceShipController* placeShipController;
public:
    InitiateOngoingGameSubstate(StateContext& context);
    ~InitiateOngoingGameSubstate();
    void openSubstate() override;
    void updateSubstate() override;
    void closeSubstate() override;
    OngoingGameState* transitToSubstate() override;
};