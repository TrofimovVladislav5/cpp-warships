#pragma once
#include <functional>

#include "GameStateDTO.h"

class StateContext {
public:
    StateContext();
    GameStateDTO* currentMatch;
    std::function<void(StateContext&)> finishCallback;
    std::function<void(StateContext&)> finishMatchCallback;
};
