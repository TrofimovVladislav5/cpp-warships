#pragma once
#include <string>

#include "GameState.h"
#include "library/parser/Parser.h"
#include "view/game-states/GameMenuView.h"

class MenuGameState : public GameState{
private:
    std::string latestCommand;
    SchemeMap inputScheme;
    void handleStart(ParsedOptions options);
    void handleExit(ParsedOptions options);
public:
    MenuGameState(StateContext& context);
    void openState() override;
    void closeState() override;
    void updateState() override;
    GameState* transitToState() override;
};
