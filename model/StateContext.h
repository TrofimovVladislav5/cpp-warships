#pragma once
#include "GameStateDTO.h"

class StateContext {
public:
    StateContext();
    GameStateDTO* currentMatch;
};
