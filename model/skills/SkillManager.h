#pragma once
#include "Skill.h"
#include <queue>
#include "../game/MatchSettings.h"
#include "SkillFactory.h"

class SkillManager {
private:
    MatchSettings* settings;
    std::deque<std::string> skills;
    std::vector<std::string> availableSkills;
    std::unordered_map<std::string, SkillFactory*> factory;
    ISkill* currentSkill;
    ISkill* createSkill(const std::string& skillName);
    void randomSkill();
public:
    explicit SkillManager(MatchSettings* settings);
    const std::vector<std::string>& nameSkills();
    const std::string& canUseSkill();
    ~SkillManager();
    void addSkill();
    void applySkill();
    void status() const;
};


