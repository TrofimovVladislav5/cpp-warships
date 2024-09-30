#include "game-states/GameState.h"
#include "game-states/OngoingGameState.h"
#include "model/StateContext.h"
#include "BattleOngoingGameSubstate.h"

BattleOngoingGameSubstate::BattleOngoingGameSubstate(StateContext& context) : OngoingGameState(context) {};

BattleOngoingGameSubstate::~BattleOngoingGameSubstate() {

}

void BattleOngoingGameSubstate::openSubstate() {

}

void BattleOngoingGameSubstate::closeSubstate() {

}

void BattleOngoingGameSubstate::updateSubstate() {

}

OngoingGameState* BattleOngoingGameSubstate::transitToSubstate() {
    return nullptr;
}