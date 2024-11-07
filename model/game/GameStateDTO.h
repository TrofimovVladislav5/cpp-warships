#pragma once
#include "AttackHandler.h"
#include "MatchSettings.h"
#include "PlayerFieldState.h"
#include "ShipManager.h"

class GameStateDTO {
public:
    GameStateDTO();
    explicit GameStateDTO(MatchSettings* settings);
    ShipManager* playerManager;
    GameField* playerField;
    ShipManager* enemyManager;
    GameField* enemyField;
    std::map<int, int> shipsSize;
    MatchSettings* settings;
    int fieldSize;
    int roundNumber;
    int currentShips;
};
