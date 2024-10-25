#pragma once
#include "view/game-states/OngoingGameView.h"

class InitiateOngoingGameView : public OngoingGameView{
public:
    void displayAvailableCommands(const GameStateDTO* currentMatch) override;
    void displayOpenState() override;
    void displayCloseState() override;
};