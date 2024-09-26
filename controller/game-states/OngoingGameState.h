#pragma once
#include "GameState.h"
#include "OngoingGameState.h"

class OngoingGameState : public GameState{
private:
    std::string latestCommand;
public:

    void openState() override;
    void closeState() override;

    void updateState() override;

    GameState* transitToState() override;
};
