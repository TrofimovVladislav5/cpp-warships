#pragma once
#include "Skill.h"
#include <queue>
#include "MatchSettings.h"

class SkillManager {
private:
    std::queue<ISkill*> skills;

public:
    explicit SkillManager(MatchSettings* settings);
    ~SkillManager();
    void applySkills();
};


