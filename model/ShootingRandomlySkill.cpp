#include "ShootingRandomlySkill.h"

ShootingRandomlySkill::ShootingRandomlySkill(ShipManager* opponentManager)
    : opponentShipManager(opponentManager)
{}

void ShootingRandomlySkill::apply() {
    std::cout << "ShootingRandomlySkill" << std::endl;
    int flag = 0;
    while (flag != 1) {
        int indexShip = std::rand() % opponentShipManager->getShips().size();
        int indexSegment = std::rand() % (*opponentShipManager)[indexShip]->getLength();
        if ((*opponentShipManager)[indexShip]->getSegmentHitPoints(indexSegment) != 0) {
            (*opponentShipManager)[indexShip]->takeDamage(indexSegment, 1);
            flag = 1;
        }
        else {
            flag = 0;
        }
    }
}