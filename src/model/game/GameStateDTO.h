#pragma once
#include "MatchSettings.h"
#include "../../controller/ShipManager.h"
#include "skills/SkillManager.h"

class GameStateDTO {
public:
    explicit GameStateDTO();
    explicit GameStateDTO(MatchSettings* settings);
    ~GameStateDTO();

    std::string lastSubState;
    ShipManager* playerManager;
    SkillManager* playerSkillManager;
    GameField* playerField;
    ShipManager* enemyManager;
    GameField* enemyField;
    std::map<int, int> shipsSizes;
    MatchSettings* settings;
    bool isFinished;
    int fieldSize;
    int roundNumber;
};
