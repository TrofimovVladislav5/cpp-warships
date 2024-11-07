#pragma once
#include "game-states/GameState.h"
#include "../model/game/GameStateDTO.h"


class GameController {
private:
    GameState *currentState;
    GameStateDTO *currentMatch;
    StateContext stateContext;
    void finishGame(StateContext& context);
    bool isFinished = false;

public:
    GameController();

    ~GameController();

    void run();
};
