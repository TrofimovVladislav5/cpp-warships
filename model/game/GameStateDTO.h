#pragma once
#include "MatchSettings.h"
#include "ShipManager.h"
#include "skills/SkillManager.h"

class GameStateDTO {
public:
    GameStateDTO();
    ~GameStateDTO();
    explicit GameStateDTO(MatchSettings* settings);
    std::string lastSubState = "";
    ShipManager* playerManager = nullptr;
    SkillManager* playerSkillManager = nullptr;
    GameField* playerField = nullptr;
    ShipManager* enemyManager = nullptr;
    GameField* enemyField = nullptr;
    std::map<int, int> shipsSizes = {};
    MatchSettings* settings = nullptr;
    int fieldSize = 0;
    int roundNumber = 0;
};
