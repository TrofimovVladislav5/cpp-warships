#pragma once
#include "game-states/GameState.h"
#include "view/GameView.h"

class GameController {
private:
    GameState *currentState;
    GameView *view;

public:
    GameController();

    ~GameController();

    void run();
};
