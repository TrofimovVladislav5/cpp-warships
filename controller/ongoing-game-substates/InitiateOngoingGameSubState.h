#pragma once
#include "OngoingGameSubState.h"
#include "PlaceShipControllerView.h"
#include "../PlaceShipController.h"

class InitiateOngoingGameSubState : public OngoingGameSubState {
private:
    SchemeMap inputScheme;
    PlaceShipController* playerPlaceController;
    PlaceShipController* enemyPlaceController;
    PlaceShipControllerView* placeControllerView;
    void handleShipsShuffle(ParsedOptions options);
    void handleConfirm(ParsedOptions options);
    bool confirmed = false;
public:
    explicit InitiateOngoingGameSubState(SubStateContext& context);
    ~InitiateOngoingGameSubState() override;
    void openSubState() override;
    void updateSubState() override;
    void closeSubState() override;
    OngoingGameSubState* transitToSubState() override;
};