#include <iostream>
#include "GameState.h"
#include "OngoingGameState.h"

#include "TypesHelper.h"
#include "save/GameSaveCreator.h"
#include "ViewHelper.h"
#include "ongoing-game-substates/BattleOngoingGameSubState.h"
#include "ongoing-game-substates/FinishOngoingGameSubState.h"
#include "ongoing-game-substates/InitiateOngoingGameSubState.h"
#include "ongoing-game-substates/NewMatchSettingsSubState.h"

OngoingGameState::OngoingGameState(StateContext& context)
    : GameState(context)
    , currentSubState(nullptr)

{
    if (!context.loadFileName.empty()) {
        loadFileName = context.loadFileName;
    }
    else {
        loadFileName = "";
    }
}

void OngoingGameState::openState() {
}

void OngoingGameState::updateState() {
    SubStateContext subStateContext{};
    if (!loadFileName.empty()) {
        GameSaveCreator saveCreator;
        subStateContext.matchDTO = saveCreator.loadSave(context.loadFileName);
        context.currentMatch = subStateContext.matchDTO;
        if (subStateContext.matchDTO->lastSubState == "BattleOngoingGameSubState") {
            currentSubState = new BattleOngoingGameSubState(subStateContext);
        } else if (subStateContext.matchDTO->lastSubState == "InitiateOngoingGameSubState") {
            currentSubState = new InitiateOngoingGameSubState(subStateContext);
        } else if (subStateContext.matchDTO->lastSubState == "FinishOngoingGameSubState") {
            currentSubState = new FinishOngoingGameSubState(subStateContext);
        }
    } else {
        currentSubState = new NewMatchSettingsSubState(subStateContext);
    }
    while (typeid(*currentSubState).name() != typeid(FinishOngoingGameSubState).name()) {
        OngoingGameSubState* newSubState = currentSubState->transitToSubState();
        if (newSubState) {
            currentSubState->closeSubState();
            currentSubState = newSubState;
            currentSubState->openSubState();
        }
        currentSubState->updateSubState();
    }
}

void OngoingGameState::closeState() {
    ViewHelper::consoleOut("Quit the game");
}

GameState* OngoingGameState::transitToState() {
    return nullptr;
}