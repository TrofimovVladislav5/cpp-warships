#include "FinishOngoingGameSubState.h"

#include "StateMessages.h"
#include "TypesHelper.h"
#include "ViewHelper.h"

FinishOngoingGameSubState::FinishOngoingGameSubState(SubStateContext* context)
    : OngoingGameSubState(context)
{}

void FinishOngoingGameSubState::openSubState() {
    std::string welcomeMessage = std::string("You have finished the game with the score of ")
        .append(std::to_string(context->matchDTO->roundNumber))
        .append(" rounds completed");

    ViewHelper::consoleOut(welcomeMessage, 1);
}

void FinishOngoingGameSubState::updateSubState() {
}

void FinishOngoingGameSubState::closeSubState() {
}

OngoingGameSubState* FinishOngoingGameSubState::transitToSubState() {
    return nullptr;
}
