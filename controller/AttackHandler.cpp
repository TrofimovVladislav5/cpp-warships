#include "AttackHandler.h"

#include "ViewHelper.h"
#include "exceptions/BattleException.h"

AttackHandler::AttackHandler(GameField* enemyField, MatchSettings* settings)
    : settings(settings)
    , enemyField(enemyField)
{}

AttackHandler::~AttackHandler() {
    delete enemyField;
    delete settings;
}

AttackResult AttackHandler::attack(std::pair<int, int> initialCoordinate) {
    int finalDamage = settings->damageCount;
    if (settings->isActiveDoubleDamage) finalDamage *= 2;
    if (initialCoordinate.first < 0 || initialCoordinate.first >= settings->getFieldSize() ||
        initialCoordinate.second < 0 || initialCoordinate.second >= settings->getFieldSize()) {
        return AttackResult::outOfBounds;
    }
    AttackResult result = enemyField->attack(initialCoordinate, finalDamage);

    if (settings->isActiveDoubleDamage) {
        settings->isActiveDoubleDamage = false;
        settings->damageCount = 1;
    }
    return result;
}
