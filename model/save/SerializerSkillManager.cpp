#include "SerializerSkillManager.h"

json SerializerSkillManager::serialize(const GameStateDTO &object) {
    json j = json{
        {"skills", json::array()}
    };
    if (!object.playerSkillManager->getSkillsQueue().empty()) {
        j["skills"] = object.playerSkillManager->getSkillsQueue();
    }
    return j;
}

void SerializerSkillManager::deserialize(const json &j, GameStateDTO &object) {
    if (object.enemyField == nullptr || object.settings == nullptr || object.enemyManager == nullptr) {
        object.playerSkillManager = nullptr;
        return;
    }
    std::deque<std::string> skills = {};
    if (j.contains("skills") && j["skills"].is_array()) {
        for (const auto& skill : j["skills"]) {
            skills.push_back(skill.get<std::string>());
        }
    }

    SkillManager* skillManager = new SkillManager(skills, object.enemyField, object.settings, object.enemyManager);
    skillManager->setSkills(skills);
    object.playerSkillManager = skillManager;
}


