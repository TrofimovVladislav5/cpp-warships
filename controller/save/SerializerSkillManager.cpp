#include "SerializerSkillManager.h"

json SerializerSkillManager::serialize(const GameStateDTO &object) {
    json j = (object.playerSkillManager->getSkillsQueue().empty() ? json{} : json{
        {"skills", object.playerSkillManager->getSkillsQueue()}
    });
    return j;
}

void SerializerSkillManager::deserialize(const json &j, GameStateDTO &object) {
    std::deque<std::string> skills = {};
    if (j.contains("skills") && j["skills"].is_array()) {
        for (const auto& skill : j["skills"]) {
            skills.push_back(skill.get<std::string>());
        }
    }

    SkillManager* skillManager = new SkillManager(skills, object.enemyField, object.settings);
    object.playerSkillManager = skillManager;
}



