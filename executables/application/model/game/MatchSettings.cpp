#include "MatchSettings.h"

#include "../../controller/ShipManager.h"

namespace cpp_warships::application {

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
} // namespace cpp_warships::application