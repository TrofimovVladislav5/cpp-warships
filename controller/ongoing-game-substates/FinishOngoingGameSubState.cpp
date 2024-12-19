#include "FinishOngoingGameSubState.h"

#include "ViewHelper.h"

FinishOngoingGameSubState::FinishOngoingGameSubState(SubStateContext& context)
    : OngoingGameSubState(context)
{
    ViewHelper::consoleOut("FinishGameState");
}

void FinishOngoingGameSubState::openSubState() {
}

void FinishOngoingGameSubState::updateSubState() {
}

void FinishOngoingGameSubState::closeSubState() {
}

OngoingGameSubState* FinishOngoingGameSubState::transitToSubState() {
    return nullptr;
}
