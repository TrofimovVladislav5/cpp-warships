#pragma once
#include "GameState.h"
#include "OngoingGameState.h"

class OngoingGameState : public GameState{
private:
    std::string latestCommand;
public:
    OngoingGameState(StateContext& context);

    void openState() override;
    void closeState() override;
    void updateState() override;
    GameState* transitToState() override;
};
