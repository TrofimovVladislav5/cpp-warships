#include "game-states/GameState.h"
#include "game-states/OngoingGameState.h"
#include "model/StateContext.h"
#include "BattleOngoingGameSubstate.h"
#include "FinishOngoingGameSubstate.h"
#include "view/ViewHelper.h"

BattleOngoingGameSubstate::BattleOngoingGameSubstate(StateContext& context) : OngoingGameState(context) {};

BattleOngoingGameSubstate::~BattleOngoingGameSubstate() {

}

void BattleOngoingGameSubstate::openSubstate() {
    ViewHelper::consoleOut("Welcome to battle. Your target to destroy enemies ships");
}

void BattleOngoingGameSubstate::closeSubstate() {
    ViewHelper::consoleOut("Battle is ended");
}

void BattleOngoingGameSubstate::updateSubstate() {
    // battleController->attackByPlayer();
    // battleController->attackByComputer();
}

OngoingGameState* BattleOngoingGameSubstate::transitToSubstate() {
    return nullptr;
}