#pragma once

#include "MatchSettings.h"
#include "Skill.h"

class Scanner : public ISkill {
private:
    MatchSettings* settings;
    std::pair<int, int> leftUpper;
    std::pair<int, int> rightLower;
public:
    Scanner(std::pair<int,int>& leftUpper, MatchSettings* settings);
    void apply() override;
};
