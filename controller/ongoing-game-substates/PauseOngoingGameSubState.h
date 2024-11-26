#pragma once
#include "OngoingGameSubState.h"
#include "Parser.h"


class PauseOngoingGameSubState final : public OngoingGameSubState {
private:
    SchemeMap inputScheme;
    std::string latestCommand;
    void handleResume(ParsedOptions options);
public:
    PauseOngoingGameSubState(SubStateContext& context);
    void openSubState() override;
    void closeSubState() override;
    void updateSubState() override;
    OngoingGameSubState* transitToSubState() override;
};
