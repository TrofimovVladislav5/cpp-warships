#include <nlohmann/json.hpp>
#include "model/game/Segment.h"
#include "model/game/Ship.h"
#include "model/game/GameField.h"
#include "model/game/GameStateDTO.h"

#include <random>
#include <iostream>

#include "BattleController.h"
#include "GameFieldView.h"
#include "Initials.h"
#include "SubStateContext.h"
#include "ongoing-game-substates/BattleOngoingGameSubState.h"
#include "ongoing-game-substates/FinishOngoingGameSubState.h"
#include "ongoing-game-substates/InitiateOngoingGameSubState.h"
#include "ongoing-game-substates/NewMatchSettingsSubState.h"

int main() {
    Initials::consoleOutInitials();
    GameSaveCreator saveCreator;
    GameStateDTO* newDTO = saveCreator.loadSave("check.json");
    std::cout << "GameStateDTO saved to save.json\n";
    std::cout << "Loaded GameStateDTO field size: " << newDTO->roundNumber << std::endl;
    std::cout << "Loaded GameStateDTO round number: " << newDTO->fieldSize << std::endl;
    for (auto& ship : newDTO->playerManager->getShips()) {
        std::cout << ship->status() << std::endl;
    }
    for (auto& ship : newDTO->enemyManager->getShips()) {
        std::cout << ship->status() << std::endl;
    }
    std::cout << newDTO->settings->damageCount << std::endl;
    SubStateContext context;
    context.matchDTO = newDTO;
    OngoingGameSubState* currentSubState = new InitiateOngoingGameSubState(context);
    while (typeid(*currentSubState).name() != typeid(FinishOngoingGameSubState).name()) {
        OngoingGameSubState* newSubState = currentSubState->transitToSubState();
        if (newSubState) {
            currentSubState->closeSubState();
            currentSubState = newSubState;
            currentSubState->openSubState();
        }
        currentSubState->updateSubState();
    }
    return 0;
}
