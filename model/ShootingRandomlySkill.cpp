#include "ShootingRandomlySkill.h"
#include <random>

#include "SkillException.h"

ShootingRandomlySkill::ShootingRandomlySkill(MatchSettings* settings)
    : settings(settings)
{}

void ShootingRandomlySkill::apply() {
    std::mt19937 mersenneEngine(std::random_device{}());

    bool success = false;
    auto& ships = settings->getOpponentManager()->getShips();

    if (ships.empty()) {
        throw SkillException("Unreachable state");
    }

    while (!success) {
        std::uniform_int_distribution<> distShips(0, ships.size() - 1);
        int indexShip = distShips(mersenneEngine);

        Ship* selectedShip = ships[indexShip];
        if (selectedShip->isDestroyed()) { continue;}

        std::uniform_int_distribution<> distSegment(0, selectedShip->getLength() - 1);
        int indexSegment = distSegment(mersenneEngine);

        if (selectedShip->getSegmentHitPoints(indexSegment) != 0) {
            selectedShip->takeDamage(indexSegment, 1);
            success = true;
        }
    }
}