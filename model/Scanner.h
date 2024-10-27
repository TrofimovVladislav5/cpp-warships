#pragma once

#include "MatchSettings.h"
#include "Skill.h"

class Scanner : public ISkill {
private:
    MatchSettings* settings;
    std::pair<int, int> leftUpper;
public:
    Scanner(const std::pair<int,int>& leftUpper, MatchSettings* settings);
    void apply() override;
};
