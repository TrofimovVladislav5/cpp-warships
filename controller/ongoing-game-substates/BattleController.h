#pragma once
#include "model/StateContext.h"

class BattleController {
private:
    StateContext& context;
public:
    BattleController(StateContext& context);
    bool attackByPlayer();
    void attackByComputer();
};