#pragma once
#include <functional>
#include "game/GameStateDTO.h"

class SubStateContext {
public:
    SubStateContext() = default;
    GameStateDTO* matchDTO = nullptr;
    std::function<void(SubStateContext&)> pauseCallback = nullptr;
};
