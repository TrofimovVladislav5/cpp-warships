#pragma once
#include "Skill.h"
#include "game/GameField.h"

class ShipManager;

class ShootingRandomlySkill : public ISkill  {
private:
     GameField* enemyField;
public:
     explicit ShootingRandomlySkill(GameField* enemyField);
     void apply() override;
};
