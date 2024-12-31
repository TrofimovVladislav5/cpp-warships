#pragma once
#include <string>

#include "GameState.h"
#include "MatchBuilder.h"
#include "library/parser/Parser.h"

class MenuGameState : public GameState{
private:
    std::string latestCommand;
    SchemeMap<void> inputScheme;
    MatchBuilder* matchBuilder;
    bool isRunning;

    void handleGameLoad(ParsedOptions options);
    void handleNewGame(ParsedOptions options);
    void handleConfirm(ParsedOptions options);
    void handleInfo(ParsedOptions options);
    void handleList(ParsedOptions options);
public:
    explicit MenuGameState(StateContext& context);
    void openState() override;
    void closeState() override;
    void updateState() override;
    GameState* transitToState() override;
};
