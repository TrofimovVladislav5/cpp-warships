#pragma once
#include "game-states/GameState.h"
#include "model/GameStateDTO.h"
#include "view/GameView.h"

class GameController {
private:
    GameState *currentState;
    GameView *view;
    GameStateDTO *currentMatch;
    StateContext stateContext;
    void finishGame(StateContext& context);
    bool isFinished = false;

public:
    GameController();

    ~GameController();

    void run();
};
