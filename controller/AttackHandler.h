#pragma once
#include "game/GameField.h"
#include "game/MatchSettings.h"

class AttackHandler {
private:
    MatchSettings* settings;
    GameField* enemyField;
public:
    explicit AttackHandler(GameField* enemyField, MatchSettings* settings);
    ~AttackHandler();
    AttackResult attack(std::pair<int, int> initialCoordinate);
};