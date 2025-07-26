#include "ShootingRandomlySkill.h"

#include <random>

#include "../../controller/ShipManager.h"
#include "../exceptions/SkillException.h"

namespace cpp_warships::application {

    ShootingRandomlySkill::ShootingRandomlySkill(GameField* enemyField)
        : enemyField(enemyField)
    {}

    void ShootingRandomlySkill::apply() {
        std::mt19937 mersenneEngine(std::random_device{}());
        bool success = false;
        while (!success) {
            auto& shipsMap = enemyField->getShipsCoordinateMap();
            std::uniform_int_distribution<> distShips(0, shipsMap.size() - 1);
            auto it = shipsMap.begin();
            std::advance(it, distShips(mersenneEngine));

            Ship* selectedShip = it->first;
            auto& coordinates = it->second;

            if (selectedShip->isDestroyed()) {
                continue;
            }

            std::vector<std::pair<int, int>> segments(coordinates.begin(), coordinates.end());
            std::uniform_int_distribution<> distSegment(0, segments.size() - 1);
            int indexSegment = distSegment(mersenneEngine);
            auto targetCoordinate = segments[indexSegment];

            if (selectedShip->getSegmentHitPoints(indexSegment) > 0) {
                enemyField->attack(targetCoordinate, 1);
                success = true;
            }
        }
    }
} // namespace cpp_warships::application
