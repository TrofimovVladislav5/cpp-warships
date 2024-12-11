#pragma once
#include <functional>

#include "game/GameStateDTO.h"

class StateContext {
public:
    StateContext();
    std::string loadFileName;
    GameStateDTO* currentMatch;
    std::function<void(StateContext&)> finishCallback;
    std::function<void(StateContext&)> finishMatchCallback;
};
