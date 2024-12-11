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
{
    if (!context.loadFileName.empty()) {
        GameSaveCreator saveCreator;
        SubStateContext subStateContext{};
        subStateContext.matchDTO = saveCreator.loadSave(context.loadFileName);
        if (subStateContext.matchDTO == nullptr) {
            currentSubState = nullptr;
            return;
        }
        context.currentMatch = subStateContext.matchDTO;
        std::cout << subStateContext.matchDTO->lastSubState << std::endl;
        if (subStateContext.matchDTO->lastSubState == "BattleOngoingGameSubState") {
            currentSubState = new BattleOngoingGameSubState(subStateContext);
        } else if (subStateContext.matchDTO->lastSubState == "InitiateOngoingGameSubState") {
            currentSubState = new InitiateOngoingGameSubState(subStateContext);
        } else if (subStateContext.matchDTO->lastSubState == "FinishOngoingGameSubState") {
            currentSubState = new FinishOngoingGameSubState(subStateContext);
        }
        currentSubState->openSubState();
    }
    else {
        currentSubState = nullptr;
    }
}

void OngoingGameState::openState() {
}

void OngoingGameState::updateState() {
    if (!currentSubState) {
        SubStateContext subStateContext{ };
        currentSubState = new NewMatchSettingsSubState(subStateContext);
        currentSubState->openSubState();
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