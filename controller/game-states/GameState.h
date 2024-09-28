#pragma once
#include "model/StateContext.h"
#include "view/GameView.h"

class GameState{
protected:
    GameView* view;
    StateContext& context;
public:
    GameState(StateContext& context);
    ~GameState();

    virtual void openState();
    virtual void updateState();
    virtual void closeState();
    virtual GameState* transitToState();
};
