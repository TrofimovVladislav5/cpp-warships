#include "../include/MatchSettings.h"

#include <iostream>

#include "../include/ShipManager.h"

namespace cpp_warships::application {

    MatchSettings::MatchSettings(std::map<int, int> shipSizesDictionary, int fieldSize)
        : shipSizesDictionary(std::move(shipSizesDictionary))
        , fieldSize(fieldSize)
        , damageCount(1)
        , isActiveDoubleDamage(false)
    {
        std::cout << "Match settings created with damage count: " << std::to_string(damageCount) << std::endl;
    }

    int MatchSettings::getFieldSize() const {
        return fieldSize;
    }

    bool MatchSettings::getIsDoubleDamage() const {
        return isActiveDoubleDamage;
    }

    int MatchSettings::getDamageCount() const {
        return damageCount;
    }

    void MatchSettings::toggleDoubleDamage(bool isActive) {
        isActiveDoubleDamage = isActive;

        if (isActive) {
            damageCount = 2;
        } else {
            damageCount = 1;
        }
    }

    std::map<int, int> MatchSettings::getShipsCount() const {
        return shipSizesDictionary;
    }
} // namespace cpp_warships::application