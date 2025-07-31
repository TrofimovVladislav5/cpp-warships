#include "../../include/player_skills/SkillManager.h"

#include <random>
#include <algorithm>
#include <iostream>
#include <cpp_warships/utilities/include/ViewHelper.h>

#include "../../include/GameStateDTO.h"
#include "../../include/player_skills/DoubleDamage.h"
#include "../../include/player_skills/ShootingRandomlySkill.h"
#include "../../include/player_skills/Scanner.h"
#include "../../include/player_skills/SkillFactory.h"
#include "../../include/player_skills/exceptions/SkillException.h"

namespace cpp_warships::application {

    SkillManager::SkillManager(GameField* enemyField, MatchSettings* settings)
        : currentSkill(nullptr)
        , skills({})
    {
        factory["Scanner"] = new ConcreteSkillFactory<Scanner, GameField*>(enemyField);
        factory["DoubleDamage"] =  new ConcreteSkillFactory<DoubleDamage, MatchSettings*>(settings);
        factory["Shooting"] = new ConcreteSkillFactory<ShootingRandomlySkill, GameField*>(enemyField);

        availableSkills = {"Scanner", "DoubleDamage", "Shooting"};

        std::shuffle(availableSkills.begin(), availableSkills.end(), std::random_device());
        for (const auto& skillName : availableSkills) {
            skills.emplace_back(skillName);
        }
    }

    SkillManager::SkillManager(const std::deque<std::string>& skills, GameField* enemyField, MatchSettings* settings)
        : skills(skills)
        , currentSkill(nullptr)
    {
        factory["Scanner"] = new ConcreteSkillFactory<Scanner, GameField*>(enemyField);
        factory["DoubleDamage"] =  new ConcreteSkillFactory<DoubleDamage, MatchSettings*>(settings);
        factory["Shooting"] = new ConcreteSkillFactory<ShootingRandomlySkill, GameField*>(enemyField);
        availableSkills = {"Scanner", "DoubleDamage", "Shooting"};
    }

    SkillManager::~SkillManager() {
        for (auto& pair : factory) {
            delete pair.second;
        }
    }

    void SkillManager::randomSkill() {
        std::random_device seed;
        std::mt19937 mersenneEngine{seed()};
        std::uniform_int_distribution<> dist{0,2};
        skills.emplace_back(availableSkills[dist(mersenneEngine)]);
    }

    const std::vector<std::string>& SkillManager::nameSkills() {
        return availableSkills;
    }
    std::string SkillManager::availableSkill() {
        if (skills.empty()) {
            return "No available skill";
        }
        return skills.front();
    }

    ISkill *SkillManager::createSkill(const std::string &skillName) {
        if (factory.find(skillName) == factory.end()) {
            throw SkillException("(where create skill) no factory for skill " + skillName + " not found");
        }
        return factory[skillName]->createSkill();
    }

    void SkillManager::applySkill() {
        if (skills.empty()) {
            throw SkillException("(where apply skill) no available skills");
        }
        currentSkill = createSkill(skills.front());
        currentSkill->apply();
        skills.pop_front();
    }

    void SkillManager::addSkill() {
        randomSkill();
    }

    void SkillManager::status() const {
        ViewHelper::consoleOut((skills.empty()) ? "No available skill"
                        :"Available skill to apply" + skills.front());
    }

    std::deque<std::string> SkillManager::getSkillsQueue() const {
        return skills;
    }

    void SkillManager::setSkills(const std::deque<std::string>& dequeSkills) {
        skills = dequeSkills;
    }
} // namespace cpp_warships::application