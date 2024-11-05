#pragma once
#include "game-states/OngoingGameState.h"
#include "CommonGameSettingsController.h"

class CommonGameSettingsSubstate : public OngoingGameState {
private:
    SchemeMap inputScheme;
    CommonGameSettingsController* controller;
    void handleConfirm(ParsedOptions options);
public:
    CommonGameSettingsSubstate(StateContext& context);
    ~CommonGameSettingsSubstate() override;
    void openSubstate() override;
    void closeSubstate() override;
    void updateSubstate() override;
    OngoingGameState* transitToSubstate() override;
};