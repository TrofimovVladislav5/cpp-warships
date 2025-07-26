#pragma once

#include "OngoingGameSubState.h"
#include "../../view/PlaceShipControllerView.h"
#include "../PlaceShipController.h"

class InitiateOngoingGameSubState : public OngoingGameSubState {
private:
    cpp_warships::input_parser::SchemeMap<void> inputScheme;
    PlaceShipController* playerPlaceController;
    PlaceShipController* enemyPlaceController;
    PlaceShipControllerView* placeControllerView;
    void handleShipsShuffle(cpp_warships::input_parser::ParsedOptions options);
    void handleConfirm(cpp_warships::input_parser::ParsedOptions options);
    void handlePause(cpp_warships::input_parser::ParsedOptions options);
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