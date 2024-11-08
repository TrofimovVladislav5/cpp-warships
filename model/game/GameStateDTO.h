#pragma once
#include "MatchSettings.h"
#include "ShipManager.h"
#include "skills/SkillManager.h"

class GameStateDTO {
public:
    GameStateDTO();
    explicit GameStateDTO(MatchSettings* settings);
    ShipManager* playerManager;
    SkillManager* playerSkillManager;
    GameField* playerField;
    ShipManager* enemyManager;
    GameField* enemyField;
    std::map<int, int> shipsSize;
    MatchSettings* settings;
    int fieldSize;
    int roundNumber;
    int currentShips;
};
