#pragma once
#include "../../model/game/GameStateDTO.h"

class GameView {
public:
    virtual ~GameView() = default;
    virtual void displayAvailableCommands(const GameStateDTO* currentMatchData) = 0;
    virtual void displayOpenState() = 0;
    virtual void displayCloseState() = 0;
};
