#include "ShootingRandomlySkill.h"

ShootingRandomlySkill::ShootingRandomlySkill(ShipManager* opponentManager)
    : opponentShipManager(opponentManager)
{}

bool isAllDestroyed(ShipManager& manager) {
    for (Ship* ship : manager.getShips()) {
        if (!ship->isDestroyed()) {
            return false;
        }
    }
    return true;
}

void ShootingRandomlySkill::apply() {

    if (isAllDestroyed(*opponentShipManager)){
        std::cout << "EXCEPTION SKILLS SHIPS DESTROYED" << std::endl;
        return;
    }

    bool success = false;
    while (!success) {
        int indexShip = std::rand() % (opponentShipManager->getShips().size() - 1);
        Ship* selectedShip = (*opponentShipManager)[indexShip];
        if (selectedShip->isDestroyed()) {
            continue;
        }

        int indexSegment = std::rand() % (selectedShip->getLength() - 1);
        if (selectedShip->getSegmentHitPoints(indexSegment) != 0) {
            selectedShip->takeDamage(indexSegment, 1);
            success = true;
        }
    }
}