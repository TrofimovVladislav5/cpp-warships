#pragma once
#include <string>

#include "GameState.h"
#include "view/GameMenuView.h"

class MenuGameState : public GameState{
private:
    std::string latestCommand;
    GameMenuView* menuView;
public:
    MenuGameState(StateContext& context);
    ~MenuGameState();

    void openState() override;
    void closeState() override;
    void updateState() override;
    GameState* transitToState() override;
};
