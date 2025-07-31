#pragma once

#include "GameField.h"
#include "MatchSettings.h"

namespace cpp_warships::application {

    class AttackHandler {
    private:
        MatchSettings* settings;
        GameField* enemyField;

    public:
        explicit AttackHandler(GameField* enemyField, MatchSettings* settings);
        ~AttackHandler();
        AttackResult attack(std::pair<int, int> initialCoordinate);
    };
}