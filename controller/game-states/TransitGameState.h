#pragma once
#include "GameState.h"
#include "Parser.h"
#include "ParserCommandInfo.h"


class TransitGameState final : public GameState {
private:
    void handleExit(ParsedOptions options);
    void handleMenu(ParsedOptions options);
    std::string latestCommand;
    SchemeMap inputScheme;
public:
    explicit TransitGameState(StateContext &context);
    void openState() override;
    void updateState() override;
    void closeState() override;
    GameState* transitToState() override;
};
