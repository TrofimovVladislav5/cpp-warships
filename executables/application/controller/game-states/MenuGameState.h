#pragma once
#include <string>

#include "GameState.h"
#include "../MatchBuilder.h"
#include <cpp_warships/input_parser/include/model/Parser.h>

class MenuGameState : public GameState{
private:
    std::string latestCommand;
    cpp_warships::input_parser::SchemeMap<void> inputScheme;
    MatchBuilder* matchBuilder;
    bool isRunning;

    void handleGameLoad(cpp_warships::input_parser::ParsedOptions options);
    void handleNewGame(cpp_warships::input_parser::ParsedOptions options);
    void handleConfirm(cpp_warships::input_parser::ParsedOptions options);
    void handleInfo(cpp_warships::input_parser::ParsedOptions options);
    void handleList(cpp_warships::input_parser::ParsedOptions options);
public:
    explicit MenuGameState(StateContext& context);
    void openState() override;
    void closeState() override;
    void updateState() override;
    GameState* transitToState() override;
};
