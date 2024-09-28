#pragma once

#include <iostream>
#include "game-states/GameState.h"

class PauseOngoingGameSubstate : public GameState {
private:
    std::string lastCommand;
public:
    void openState() override;
    void closeState() override;
    void updateState() override;
    GameState* transitToState() override;
};
