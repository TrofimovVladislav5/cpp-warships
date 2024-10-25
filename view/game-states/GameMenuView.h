#pragma once
#include "GameView.h"

class GameMenuView : public GameView {
public:
    void displayAvailableCommands(const GameStateDTO* currentMatch) override;
    void displayOpenState() override;
    void displayCloseState() override;
};