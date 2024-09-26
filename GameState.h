#pragma once
#include "GameView.h"

class GameState{
protected:
    GameView* view;
public:
    virtual void openState();
    virtual void updateState();
    virtual void closeState();
    virtual GameState* transitToState();
};