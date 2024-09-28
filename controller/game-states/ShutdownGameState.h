#pragma once
#include "GameState.h"

class ShutdownGameState : public GameState {
public:
    ShutdownGameState(StateContext& context);
    ~ShutdownGameState();

    void openState() override;
    void updateState() override;
    void closeState() override;
    GameState* transitToState() override;
};
