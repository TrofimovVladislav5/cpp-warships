#include "DoubleDamageSkill.h"
#include <iostream>
DoubleDamageSkill::DoubleDamageSkill(std::pair<int, int> coord, GameField *gameField)
    : attackCoordinate(coord)
    , gameField(gameField)
{}

void DoubleDamageSkill::apply() {
    std::cout << "DoubleDamage" << std::endl;
    gameField->attack(attackCoordinate, 2);
}
