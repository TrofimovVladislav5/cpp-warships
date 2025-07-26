#pragma once
#include "GameView.h"

class OngoingGameView : public GameView {
public:
    virtual void displayAvailableCommands(const GameStateDTO* currentMatchData);
    virtual void displayOpenState();
    virtual void displayCloseState();
};
