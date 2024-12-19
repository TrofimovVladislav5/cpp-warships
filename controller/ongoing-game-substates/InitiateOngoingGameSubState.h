#pragma once
#include "OngoingGameSubState.h"
#include "PlaceShipControllerView.h"
#include "../PlaceShipController.h"

class InitiateOngoingGameSubState : public OngoingGameSubState {
private:
    SchemeMap<void> inputScheme;
    PlaceShipController* playerPlaceController;
    PlaceShipController* enemyPlaceController;
    PlaceShipControllerView* placeControllerView;
    void handleShipsShuffle(ParsedOptions options);
    void handleConfirm(ParsedOptions options);
    void handlePause(ParsedOptions options);
    std::string latestCommand;
    bool confirmed;
public:
    explicit InitiateOngoingGameSubState(SubStateContext* context);
    ~InitiateOngoingGameSubState() override;
    void openSubState() override;
    void updateSubState() override;
    void closeSubState() override;
    OngoingGameSubState* transitToSubState() override;
};