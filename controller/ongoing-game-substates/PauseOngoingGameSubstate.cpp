#include "PauseOngoingGameSubstate.h"

#include "game-states/MenuGameState.h"
#include "game-states/OngoingGameState.h"
#include "view/ViewHelper.h"

void PauseOngoingGameSubstate::openState(){
    ViewHelper::consoleOut("Game on pause.\nChoose Resume Menu");
}
void PauseOngoingGameSubstate::closeState() {
    ViewHelper::consoleOut("Quit the Pause");
}
void PauseOngoingGameSubstate::updateState() {
    std::getline(std::cin, lastCommand);
}
GameState* PauseOngoingGameSubstate::transitToState() {
    if (lastCommand == "resume") {
        return new OngoingGameState(context);
    }
    else if (lastCommand == "menu") {
        return new MenuGameState(context);
    }
    return nullptr;
}