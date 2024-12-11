#pragma once
#include <string>
#include "GameState.h"
#include "library/parser/Parser.h"
#include "model/StateContext.h"
#include "controller/ongoing-game-substates/OngoingGameSubState.h"

class OngoingGameState : public GameState {
protected:
    std::string latestCommand;
    OngoingGameSubState* currentSubState;
public:
    OngoingGameState(StateContext &context);
    void openState() override;
    void updateState() override;
    void closeState() override;
    GameState* transitToState() override;
};
