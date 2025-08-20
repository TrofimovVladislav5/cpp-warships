#pragma once

#include "Skill.h"
#include "../GameField.h"

namespace cpp_warships::application {

     class ShipManager;

     class ShootingRandomlySkill : public ISkill  {
     private:
          GameField* enemyField;
     public:
          explicit ShootingRandomlySkill(GameField* enemyField);
          void apply() override;
     };
} // namespace cpp_warships::application