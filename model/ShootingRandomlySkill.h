#pragma once
#include "MatchSettings.h"
#include "Skill.h"

class ShootingRandomlySkill : public ISkill  {
private:
     MatchSettings* settings;
public:
     explicit ShootingRandomlySkill(MatchSettings* settings);
     void apply() override;
};
