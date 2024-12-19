#pragma once
#include "../MatchSettingsController.h"
#include "OngoingGameSubState.h"

class NewMatchSettingsSubState : public OngoingGameSubState {
private:
    SchemeMap<void> inputScheme;
    MatchSettingsController* controller;
    bool isConfirmed = false;
    MatchSettings* currentSettings;
    void handleMatchSettings(ParsedOptions options);
public:
    explicit NewMatchSettingsSubState(SubStateContext* context);
    ~NewMatchSettingsSubState() override;
    void openSubState() override;
    void closeSubState() override;
    void updateSubState() override;
    OngoingGameSubState* transitToSubState() override;
};
