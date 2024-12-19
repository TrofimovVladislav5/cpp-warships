#pragma once
#include "Skill.h"
#include "game/GameField.h"

class ShipManager;

class ShootingRandomlySkill : public ISkill  {
private:
     ShipManager* opponentShipManager;
public:
     explicit ShootingRandomlySkill(ShipManager* opponentShipManager);
     void apply() override;
};
