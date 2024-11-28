#pragma once
#include "Skill.h"
#include <queue>
#include <string>
#include "ShipManager.h"
#include "SkillFactory.h"
#include "game/GameField.h"
#include "game/MatchSettings.h"

class SkillManager {
private:
    std::deque<std::string> skills;
    std::vector<std::string> availableSkills;
    std::unordered_map<std::string, SkillFactory*> factory;
    ISkill* currentSkill;
    ISkill* createSkill(const std::string& skillName);
    void randomSkill();
public:
    explicit SkillManager(GameField* enemyField, MatchSettings* settings, ShipManager* enemyManager);
    explicit SkillManager(const std::deque<std::string>& skills, GameField* enemyField, MatchSettings* settings, ShipManager* enemyManager);
    const std::vector<std::string>& nameSkills();
    const std::string& availableSkill();
    ~SkillManager();
    void addSkill();
    void applySkill();
    void status() const;
    void setSkills(const std::deque<std::string>& dequeSkills);
    [[nodiscard]] std::deque<std::string> getSkillsQueue() const;
};


