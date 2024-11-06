#include <iostream>
#include "GameState.h"
#include "OngoingGameState.h"

#include "ViewHelper.h"
#include "ongoing-game-substates/FinishOngoingGameSubState.h"
#include "ongoing-game-substates/NewMatchSettingsSubState.h"


OngoingGameState::OngoingGameState(StateContext& context)
    : GameState(context)
    , currentSubState(nullptr)
    , ongoingGameView(nullptr)
{}

OngoingGameState::~OngoingGameState(){
    delete this->ongoingGameView;
}

void OngoingGameState::openState() {
}

void OngoingGameState::updateState() {
    SubStateContext subStateContext;
    currentSubState = new NewMatchSettingsSubState(subStateContext);
    currentSubState->openSubState();

    while (typeid(*currentSubState).name() != typeid(FinishOngoingGameSubState).name()) {
        OngoingGameSubState* newSubState = currentSubState->transitToSubState();
        if (newSubState) {
            currentSubState->closeSubState();
            currentSubState = newSubState;
            currentSubState->openSubState();
        }
        currentSubState->updateSubState();
    }
}

void OngoingGameState::closeState() {
    ViewHelper::consoleOut("Quit the game");
}

GameState* OngoingGameState::transitToState() {
    return nullptr;
}