#pragma once
#include "OngoingGameSubState.h"
#include "Parser.h"

class FinishOngoingGameSubState : public OngoingGameSubState {
public:
    explicit FinishOngoingGameSubState(SubStateContext* context);
    void openSubState() override;
    void updateSubState() override;
    void closeSubState() override;
    OngoingGameSubState* transitToSubState() override;
};
