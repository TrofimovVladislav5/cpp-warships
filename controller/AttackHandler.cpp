#include "AttackHandler.h"

AttackHandler::AttackHandler(GameField* enemyField, MatchSettings* settings)
    : settings(settings)
    , enemyField(enemyField)
{}

AttackResult AttackHandler::attack(std::pair<int, int> initialCoordinate) {
    int finalDamage = settings->damageCount;
    if (settings->isActiveDoubleDamage) finalDamage *= 2;

    AttackResult result = enemyField->attack(initialCoordinate, finalDamage);

    if (settings->isActiveDoubleDamage) {
        settings->isActiveDoubleDamage = false;
        settings->damageCount = 1;
    }

    return result;
}
