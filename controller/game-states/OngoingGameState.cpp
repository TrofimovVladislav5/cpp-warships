#include <iostream>
#include "GameState.h"
#include "OngoingGameState.h"
#include "ongoing-game-substates/InitiateOngoingGameSubstate.h"
#include "ongoing-game-substates/FinishOngoingGameSubstate.h"
#include "../../library/ViewHelper.h"
#include "../view/game-states/OngoingGameView.h"
#include "view/game-substates/InitiateOngoingGameView.h"
#include "library/parser-builder/ConfigCommandBuilder.h"
#include "ongoing-game-substates/CommonGameSettingsSubstate.h"
#include "view/game-substates/CommonGameSettingsView.h"
OngoingGameState::OngoingGameState(StateContext& context)
    : GameState(context)
    , ongoingGameView(nullptr)
{
    view = new OngoingGameView();
}

OngoingGameState::~OngoingGameState(){
    delete this->ongoingGameView;
}

void OngoingGameState::openState() {
    view->displayOpenState();
}

void OngoingGameState::updateState() {
    currentSubstate = new CommonGameSettingsSubstate(context);
    currentSubstate->openSubstate();

    while (typeid(*currentSubstate).name() != typeid(FinishOngoingGameSubstate).name()) {
        OngoingGameState* newSubstate = currentSubstate->transitToSubstate();
        if (newSubstate) {
            currentSubstate->closeSubstate();
            currentSubstate = newSubstate;
            currentSubstate->openSubstate();
        }
        currentSubstate->updateSubstate();
    }
}

void OngoingGameState::closeState() {
    ViewHelper::consoleOut("Quit the game");
}

GameState* OngoingGameState::transitToState(){
    return nullptr;
}

void OngoingGameState::closeSubstate() {};

void OngoingGameState::openSubstate() {};

void OngoingGameState::updateSubstate() {};

OngoingGameState* OngoingGameState::transitToSubstate() {
    return nullptr;
};