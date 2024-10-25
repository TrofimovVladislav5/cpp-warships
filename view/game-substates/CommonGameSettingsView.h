#pragma once
#include "OngoingGameView.h"

class CommonGameSettingsView : public OngoingGameView {
public:
    void displayAvailableCommands(const GameStateDTO* currentMatch) override;
    void displayOpenState() override;
    void displayCloseState() override;
};