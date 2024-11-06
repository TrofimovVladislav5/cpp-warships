#pragma once
#include "OngoingGameSubState.h"
#include "../PlaceShipController.h"

class InitiateOngoingGameSubState : public OngoingGameSubState {
private:
    SchemeMap inputScheme;
    PlaceShipController* placeShipController;
public:
    InitiateOngoingGameSubState(SubStateContext& context);
    ~InitiateOngoingGameSubState() override;
    void openSubState() override;
    void updateSubState() override;
    void closeSubState() override;
    OngoingGameSubState* transitToSubState() override;
};