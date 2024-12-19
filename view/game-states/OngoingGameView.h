#pragma once
#include "GameView.h"

class OngoingGameView : public GameView {
public:
    virtual void displayAvailableCommands(const GameStateDTO* currentMatch);
    virtual void displayOpenState();
    virtual void displayCloseState();
};