#pragma once

#include "OngoingGameSubState.h"
#include "../../view/PlaceShipControllerView.h"
#include "../PlaceShipController.h"

namespace cpp_warships::application {

    class InitiateOngoingGameSubState : public OngoingGameSubState {
    private:
        input_parser::SchemeMap<void> inputScheme;
        PlaceShipController* playerPlaceController;
        PlaceShipController* enemyPlaceController;
        PlaceShipControllerView* placeControllerView;
        void handleShipsShuffle(input_parser::ParsedOptions options);
        void handleConfirm(input_parser::ParsedOptions options);
        void handlePause(input_parser::ParsedOptions options);
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
} // namespace cpp_warships::application
