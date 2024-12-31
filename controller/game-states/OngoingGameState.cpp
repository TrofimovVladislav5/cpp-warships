#include <iostream>
#include "GameState.h"
#include "OngoingGameState.h"

#include "StateMessages.h"
#include "TransitGameState.h"
#include "ViewHelper.h"
#include "ongoing-game-substates/FinishOngoingGameSubState.h"


OngoingGameState::OngoingGameState(StateContext& context)
    : GameState(context)
    , currentSubState(nullptr)
{}

void OngoingGameState::openState() {
    StateMessages::displayGreetingMessage("Match");
    currentSubState = context.initialGameSubState;
}

void OngoingGameState::updateState() {
    OngoingGameSubState* newSubState = currentSubState->transitToSubState();
    if (newSubState) {
        currentSubState->closeSubState();
        currentSubState = newSubState;
        currentSubState->openSubState();
    }

    currentSubState->updateSubState();
}

void OngoingGameState::closeState() {
    ViewHelper::consoleOut("Leaving the game");
}

GameState* OngoingGameState::transitToState() {
    if (typeid(*currentSubState).name() == typeid(FinishOngoingGameSubState).name()) {
        return new TransitGameState(context);
    }

    return nullptr;
}