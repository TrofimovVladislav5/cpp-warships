#pragma once
#include <string>

#include "GameState.h"
#include "MatchBuilder.h"
#include "library/parser/Parser.h"

class MenuGameState : public GameState{
private:
    std::string latestCommand;
    SchemeMap inputScheme;
    MatchBuilder* matchBuilder;
    bool isRunning;

    void handleGameLoad(ParsedOptions options);
    void handleNewGame(ParsedOptions options);
    void handleConfirm(ParsedOptions options);
public:
    explicit MenuGameState(StateContext& context);
    void openState() override;
    void closeState() override;
    void updateState() override;
    GameState* transitToState() override;
};
