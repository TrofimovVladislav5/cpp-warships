#pragma once
#include "ShipManager.h"
#include "Skill.h"

class ShootingRandomlySkill : public ISkill  {
private:
     ShipManager* opponentShipManager;
public:
     explicit ShootingRandomlySkill(ShipManager* opponentManager);
     void apply() override;
};
