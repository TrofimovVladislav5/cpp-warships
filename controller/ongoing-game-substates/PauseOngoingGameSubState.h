#pragma once
#include "GamePauseView.h"
#include "MatchBuilder.h"
#include "OngoingGameSubState.h"
#include "Parser.h"
#include "save/GameSaveCreator.h"


class PauseOngoingGameSubState final : public OngoingGameSubState {
private:
    SchemeMap inputScheme;
    MatchBuilder matchBuilder;
    GamePauseView view;
    OngoingGameSubState* loadedSubState;
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
