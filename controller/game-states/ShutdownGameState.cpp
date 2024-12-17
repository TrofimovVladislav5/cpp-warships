#include "ShutdownGameState.h"


ShutdownGameState::ShutdownGameState(StateContext& context)
    : GameState(context)
{}

void ShutdownGameState::openState() {
    context.currentMatchData->isFinished = true;
}

void ShutdownGameState::updateState() {

}

void ShutdownGameState::closeState() {

}

GameState* ShutdownGameState::transitToState() {
    return nullptr;
}
