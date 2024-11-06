#pragma once
#include <string>
#include "GameState.h"
#include "view/game-states/OngoingGameView.h"
#include "library/parser/Parser.h"
#include "model/StateContext.h"

class OngoingGameSubState;

class OngoingGameState : public GameState {
private: 
    SchemeMap inputScheme;
protected:
    std::string latestCommand;
    OngoingGameSubState* currentSubState;
    OngoingGameView* ongoingGameView;
public:
    OngoingGameState(StateContext &context);
    virtual ~OngoingGameState();
    void openState() override;
    void updateState() override;
    void closeState() override;
    GameState* transitToState() override;
};
