#include "GameController.h"
#include "MenuGameState.h"
#include <iostream>
GameController::GameController(){
    currentState = new MenuGameState();
    view = new GameView();
}

GameController::~GameController(){
    delete currentState;
    delete view;
}

void GameController::run(){
    while (true){
        GameState* newState = currentState->transitToState();
        if(newState){
            currentState->closeState();
            currentState = newState;
            currentState->openState();
        }
        currentState->updateState();
    }
}