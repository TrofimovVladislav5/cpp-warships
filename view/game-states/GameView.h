#pragma once
#include "../../model/game/GameStateDTO.h"

class GameView {
public:
    virtual ~GameView() = default;
    virtual void displayAvailableCommands(const GameStateDTO* currentMatch) = 0;
    virtual void displayOpenState() = 0;
    virtual void displayCloseState() = 0;
};