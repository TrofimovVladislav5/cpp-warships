#include "SerializerSkillManager.h"

json SerializerSkillManager::serialize(const GameStateDTO &object) {
    json j = json{
        {"skills", object.playerSkillManager->getSkillsQueue()}
    };
    return j;
}

void SerializerSkillManager::deserialize(const json &j, GameStateDTO &object) {
    if (object.enemyField == nullptr || object.settings == nullptr || object.enemyManager == nullptr) {
        object.playerSkillManager = nullptr;
        return;
    }
    std::deque<std::string> skills;
    for (const auto& skill : j.at("skills")) {
        skills.push_back(skill.get<std::string>());
    }

    SkillManager* skillManager = new SkillManager(object.enemyField, object.settings, object.enemyManager);
    skillManager->setSkills(skills);
    object.playerSkillManager = skillManager;
}


