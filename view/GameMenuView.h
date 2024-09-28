#pragma once
#include "model/GameStateDTO.h"

class GameMenuView {
public:
    void drawAvailableCommands(const GameStateDTO* currentMatch);
};
