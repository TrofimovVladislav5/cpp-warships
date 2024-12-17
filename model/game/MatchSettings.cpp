#include "MatchSettings.h"

#include "../../controller/ShipManager.h"

MatchSettings::MatchSettings(std::map<int, int> shipSizesDictionary, int fieldSize)
    : fieldSize(fieldSize)
    , damageCount(1)
    , shipSizesDictionary(std::move(shipSizesDictionary))
{}

int MatchSettings::getFieldSize() const {
    return fieldSize;
}

std::map<int, int> MatchSettings::getShipsCount() const {
    return shipSizesDictionary;
}
