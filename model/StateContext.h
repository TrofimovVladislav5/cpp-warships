#pragma once
#include <functional>

#include "game/GameStateDTO.h"
#include "ongoing-game-substates/OngoingGameSubState.h"

class StateContext {
public:
    StateContext();
    std::string loadFileName;
    GameStateDTO* currentMatchData;
    OngoingGameSubState* initialGameSubState;
};
