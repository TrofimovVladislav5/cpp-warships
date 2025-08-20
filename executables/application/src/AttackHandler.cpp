#include "../include/AttackHandler.h"

#include <iostream>
#include <string>
#include <cpp_warships/utilities/include/ViewHelper.h>

#include "../include/exceptions/BattleException.h"

namespace cpp_warships::application {

    AttackHandler::AttackHandler(GameField* enemyField, MatchSettings* settings)
        : settings(settings)
        , enemyField(enemyField)
    {}

    AttackHandler::~AttackHandler() {
        delete enemyField;
        delete settings;
    }

    AttackResult AttackHandler::attack(std::pair<int, int> initialCoordinate) {
        AttackResult result;
        try {
            int damageCount = settings->getDamageCount();
            if (settings->getIsDoubleDamage()) {
                damageCount *= 2;
            }

            if (initialCoordinate.first < 0 || initialCoordinate.first >= settings->getFieldSize() ||
                initialCoordinate.second < 0 || initialCoordinate.second >= settings->getFieldSize()) {
                return AttackResult::outOfBounds;
            }
            result = enemyField->attack(initialCoordinate, damageCount);

            if (settings->getIsDoubleDamage()) {
                settings->toggleDoubleDamage(false);
            }
        } catch (std::out_of_range& e) {
            ViewHelper::errorOut("Wrong coordinates to attack", e);
            result = AttackResult::outOfBounds;
        }

        return result;
    }
} // namespace cpp_warships::application