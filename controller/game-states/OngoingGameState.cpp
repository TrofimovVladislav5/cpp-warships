#include <iostream>
#include "GameState.h"
#include "OngoingGameState.h"
#include "ongoing-game-substates/InitiateOngoingGameSubstate.h"
#include "ongoing-game-substates/BattleOngoingGameSubstate.h"
#include "ongoing-game-substates/PauseOngoingGameSubstate.h"
#include "ongoing-game-substates/FinishOngoingGameSubstate.h"
#include "view/ViewHelper.h"

OngoingGameState::OngoingGameState(StateContext& context) : GameState(context){

}

OngoingGameState::~OngoingGameState(){
    delete this->ongoingGameView;
}

void OngoingGameState::openState() {
    //TODO: Implement printing out the 'hello' letter (game rules, etc.)
    ViewHelper::consoleOut("Welcome to Ongoing game");
    ongoingGameView->printGameCommands();
}

void OngoingGameState::updateState() {
    //TODO: Same logic as in GameController
    currentSubstate = new InitiateOngoingGameSubstate(context);
    currentSubstate->openSubstate();
    while (typeid(*currentSubstate).name() != typeid(FinishOngoingGameSubstate).name()){
        OngoingGameState* newSubstate = currentSubstate->transitToSubstate();
        if (newSubstate){
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