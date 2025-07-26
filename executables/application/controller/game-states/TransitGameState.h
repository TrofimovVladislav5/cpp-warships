#pragma once

#include <cpp_warships/input_parser/include/model/Parser.h>
#include <cpp_warships/input_parser/include/model/ParserCommandInfo.h>

#include "GameState.h"

class TransitGameState final : public GameState {
private:
    void handleExit(cpp_warships::input_parser::ParsedOptions options);
    void handleMenu(cpp_warships::input_parser::ParsedOptions options);
    std::string latestCommand;
    cpp_warships::input_parser::SchemeMap<void> inputScheme;
public:
    explicit TransitGameState(StateContext &context);
    void openState() override;
    void updateState() override;
    void closeState() override;
    GameState* transitToState() override;
};
