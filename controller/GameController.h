#pragma once
#include "MatchBuilder.h"
#include "Parser.h"
#include "game-states/GameState.h"
#include "../model/game/GameStateDTO.h"


class GameController {
private:
    GameState *currentState;
    GameStateDTO *currentMatchData;
    StateContext stateContext;
public:
    GameController();

    ~GameController();

    void run();
};
