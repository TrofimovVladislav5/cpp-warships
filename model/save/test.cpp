#include "../cmake-build-debug/_deps/nlohmann_json-src/single_include/nlohmann/json.hpp"
#include "model/game/Segment.h"
#include "model/game/Ship.h"
#include "model/ShipManager.h"
#include "model/game/GameField.h"
#include "model/game/GameStateDTO.h"

#include "GameSaveCreator.h"
#include <random>
#include <iostream>
#include "GameFieldView.h"
#include "Initials.h"

int main() {
    Initials::consoleOutInitials();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 10);

    std::vector<Ship*> ships;
    for (int i = 0; i < 5; ++i) {
        int length = dis(gen);
        int maxHealth = dis(gen) * 10;
        ships.push_back(new Ship(length, maxHealth));
    }

    std::unordered_set<std::pair<int, int>, hashFunc> attacksOnField;
    for (int i = 0; i < 10; ++i) {
        attacksOnField.emplace(dis(gen), dis(gen));
    }

    std::unordered_map<Ship*, std::unordered_set<std::pair<int, int>, hashFunc>> shipsCoordinatesMap;
    for (auto* ship : ships) {
        std::unordered_set<std::pair<int, int>, hashFunc> coordinates;
        for (int i = 0; i < ship->getLength(); ++i) {
            coordinates.emplace(dis(gen), dis(gen));
        }
        shipsCoordinatesMap[ship] = coordinates;
    }

    GameField* playerField = new GameField(10, 10, shipsCoordinatesMap, attacksOnField);
    GameField* enemyField = new GameField(10, 10, shipsCoordinatesMap, attacksOnField);

    std::map<int, int> shipsSizes = {{4, 1}, {3, 1}};
    ShipManager* playerManager = new ShipManager(shipsSizes, ships);
    ShipManager* enemyManager = new ShipManager(shipsSizes, ships);

    MatchSettings* settings = new MatchSettings(shipsSizes, 10);

    SkillManager* playerSkillManager = new SkillManager(playerField, settings, enemyManager);

    GameStateDTO gameState;
    gameState.playerManager = playerManager;
    gameState.playerSkillManager = playerSkillManager;
    gameState.playerField = playerField;
    gameState.enemyManager = enemyManager;
    gameState.enemyField = enemyField;
    gameState.settings = settings;
    gameState.shipsSizes = shipsSizes;
    gameState.fieldSize = 10;
    gameState.roundNumber = 1;
    gameState.currentShips = 2;

    GameSaveCreator saveCreator(gameState);
    saveCreator.createSave();
    std::cout << "GameStateDTO saved to save.json\n";
    // GameSaveCreator saveCreator;
    GameStateDTO loadedGameState = saveCreator.loadSave();
    std::cout << "Loaded GameStateDTO field size: " << loadedGameState.fieldSize << std::endl;
    std::cout << "Loaded GameStateDTO round number: " << loadedGameState.roundNumber << std::endl;
    std::cout << "Loaded GameStateDTO current ships: " << loadedGameState.currentShips << std::endl;
    GameFieldView* playerFieldView = new GameFieldView(loadedGameState.playerField);
    playerFieldView->displayField(false);
    return 0;
}
