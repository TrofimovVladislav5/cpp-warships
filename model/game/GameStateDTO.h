#pragma once
#include "MatchSettings.h"
#include "ShipManager.h"
#include "skills/SkillManager.h"

class GameStateDTO {
public:
    GameStateDTO();
    explicit GameStateDTO(MatchSettings* settings);
    ShipManager* playerManager = nullptr;
    SkillManager* playerSkillManager = nullptr;
    GameField* playerField = nullptr;
    ShipManager* enemyManager = nullptr;
    GameField* enemyField = nullptr;
    std::map<int, int> shipsSizes = {};
    MatchSettings* settings = nullptr;
    int fieldSize = 0;
    int roundNumber = 0;
    int currentShips = 0;
};
