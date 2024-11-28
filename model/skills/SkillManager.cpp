#include <random>
#include <algorithm>
#include "SkillManager.h"
#include "DoubleDamage.h"
#include "ShootingRandomlySkill.h"
#include "Scanner.h"
#include "SkillFactory.h"
#include "ViewHelper.h"
#include "exceptions/SkillException.h"
#include "game/GameStateDTO.h"

SkillManager::SkillManager(GameField* enemyField, MatchSettings* settings, ShipManager* enemyManager)
    : currentSkill(nullptr)
{
    availableSkills = {
        {"Scanner"},
        {"DoubleDamage"},
        {"Shooting"}
    };

    factory["Scanner"] = new ConcreteSkillFactory<Scanner, GameField*>(enemyField);
    factory["DoubleDamage"] =  new ConcreteSkillFactory<DoubleDamage, MatchSettings*>(settings);
    factory["Shooting"] = new ConcreteSkillFactory<ShootingRandomlySkill, ShipManager*>(enemyManager);

    std::shuffle(availableSkills.begin(), availableSkills.end(), std::random_device());
    for (const auto& skillName : availableSkills) {
        skills.emplace_back(skillName);
    }
}

SkillManager::SkillManager(const std::deque<std::string>& skills, GameField* enemyField, MatchSettings* settings, ShipManager* enemyManager)
    : skills(skills)
{
    availableSkills = {
        {"Scanner"},
        {"DoubleDamage"},
        {"Shooting"}
    };

    factory["Scanner"] = new ConcreteSkillFactory<Scanner, GameField*>(enemyField);
    factory["DoubleDamage"] =  new ConcreteSkillFactory<DoubleDamage, MatchSettings*>(settings);
    factory["Shooting"] = new ConcreteSkillFactory<ShootingRandomlySkill, ShipManager*>(enemyManager);
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
const std::string& SkillManager::availableSkill() {
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