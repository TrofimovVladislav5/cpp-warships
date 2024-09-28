#include "GameState.h"

GameState::GameState(StateContext& context) : context(context) {
    this->view = new GameView;
}

GameState::~GameState() {
    delete this->view;
}

void GameState::openState(){};

void GameState::updateState(){};

void GameState::closeState(){};

GameState* GameState::transitToState() {
    return nullptr;
};
