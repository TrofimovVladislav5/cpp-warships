#include "ShutdownGameState.h"

#include "ViewHelper.h"


ShutdownGameState::ShutdownGameState(StateContext& context)
    : GameState(context)
{}

void ShutdownGameState::openState() {
    context.currentMatchData->isFinished = true;
}

void ShutdownGameState::updateState() {
    ViewHelper::consoleOut("Good good bye!");
}

void ShutdownGameState::closeState() {

}

GameState* ShutdownGameState::transitToState() {
    return nullptr;
}
