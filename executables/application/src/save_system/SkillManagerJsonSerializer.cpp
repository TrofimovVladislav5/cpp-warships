#include "../../include/save_system/SkillManagerJsonSerializer.h"

#include "../../include/player_skills/SkillManager.h"

namespace cpp_warships::application {

    bool SkillManagerJsonSerializer::isRelated(nlohmann::basic_json<> item) {
        return item.contains("authenticator") &&
               item["authenticator"].is_string() &&
               item["authenticator"] == "skillManager";
    }

    nlohmann::json SkillManagerJsonSerializer::serialize(SkillManager& item) {
        auto fieldSerializer = std::get<0>(childrenSerializers);
        auto settingsSerializer = std::get<1>(childrenSerializers);

        return nlohmann::json{
            {"authenticator", "skillManager"},
            {"availableSkills", item.getSkillsQueue()},
            {"enemyField", fieldSerializer.serialize(*item.getReferenceField())},
            {"matchSettings", settingsSerializer.serialize(*item.getMatchSettings())}
        };
    }

    SkillManager SkillManagerJsonSerializer::deserialize(nlohmann::basic_json<> item) {
        if (!isRelated(item)) {
            throw std::runtime_error("Invalid JSON for SkillManager deserialization");
        }

        auto fieldSerializer = std::get<0>(childrenSerializers);
        auto settingsSerializer = std::get<1>(childrenSerializers);

        SkillManager skillManager{
            item["availableSkills"].get<std::deque<std::string>>(),
            new GameField(fieldSerializer.deserialize(item["enemyField"])),
            new MatchSettings(settingsSerializer.deserialize(item["matchSettings"]))
        };

        return skillManager;
    }
} // namespace cpp_warships::application