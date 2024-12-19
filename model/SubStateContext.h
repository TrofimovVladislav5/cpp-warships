#pragma once
#include "game/GameStateDTO.h"

class SubStateContext {
public:
    SubStateContext() = default;
    GameStateDTO* matchDTO;
};
