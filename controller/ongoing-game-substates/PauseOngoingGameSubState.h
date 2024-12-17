#pragma once
#include "OngoingGameSubState.h"
#include "Parser.h"
#include "save/GameSaveCreator.h"


class PauseOngoingGameSubState final : public OngoingGameSubState {
private:
    GameStateDTO* dto;
    GameSaveCreator* saveCreator;
    SchemeMap inputScheme;
    std::string latestCommand;
    void handleResume(ParsedOptions options);
    void handleSave(ParsedOptions options);
    void handleLoad(ParsedOptions options);
public:
    explicit PauseOngoingGameSubState(SubStateContext* context);
    void openSubState() override;
    void closeSubState() override;
    void updateSubState() override;
    OngoingGameSubState* transitToSubState() override;
};
