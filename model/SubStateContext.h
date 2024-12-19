#pragma once
#include "game/GameStateDTO.h"

class SubStateContext {
public:
    explicit SubStateContext(GameStateDTO* matchData);
    GameStateDTO* matchDTO;
};
