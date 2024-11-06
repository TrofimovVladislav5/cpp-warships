#include "ShutdownGameState.h"


ShutdownGameState::ShutdownGameState(StateContext& context)
    : GameState(context)
{}

ShutdownGameState::~ShutdownGameState()
{}

void ShutdownGameState::openState() {
    context.finishCallback(context);
}

void ShutdownGameState::updateState() {

}

void ShutdownGameState::closeState() {

}

GameState* ShutdownGameState::transitToState() {
    return nullptr;
}
