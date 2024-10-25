#pragma once
#include "model/StateContext.h"
#include "view/game-states/GameView.h"

class GameState{
protected:
    GameView* view;
    StateContext& context;
public:
    GameState(StateContext& context);
    virtual ~GameState();
    virtual void openState() = 0;
    virtual void updateState() = 0;
    virtual void closeState() = 0;
    virtual GameState* transitToState() = 0;
};
