#include <iostream>
#include "PauseOngoingGameSubstate.h"
#include "ongoing-game-substates/BattleOngoingGameSubstate.h"
#include "game-states/OngoingGameState.h"
#include "view/ViewHelper.h"

void PauseOngoingGameSubstate::openSubstate() {
    ViewHelper::consoleOut("Battle on paused");
}

void PauseOngoingGameSubstate::closeSubstate() {
    ViewHelper::consoleOut("Match resume");
}

void PauseOngoingGameSubstate::updateSubstate() {
    ViewHelper::consoleOut("For return to battle use command resume");
    std::getline(std::cin, latestCommand);
}

OngoingGameState* PauseOngoingGameSubstate::transitToSubstate() {
    if (latestCommand == "resume") {
        return new BattleOngoingGameSubstate(context);
    }
    return nullptr;
}