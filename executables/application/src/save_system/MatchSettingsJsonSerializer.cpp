#include "../../include/save_system/MatchSettingsJsonSerializer.h"

namespace cpp_warships::application {

    bool MatchSettingsJsonSerializer::isRelated(nlohmann::basic_json<> item) {
        return item.contains("authenticator") &&
               item["authenticator"].is_string() &&
               item["authenticator"] == "matchSettings";
    }

    nlohmann::json MatchSettingsJsonSerializer::serialize(MatchSettings& item) {
        return nlohmann::json{
            {"authenticator", "matchSettings"},
            {"damageCount", item.getDamageCount()},
            {"isDoubleDamage", item.getIsDoubleDamage()},
            {"fieldSize", item.getFieldSize()},
            {"shipsDictionary", item.getShipsCount()}
        };
    }

    MatchSettings MatchSettingsJsonSerializer::deserialize(nlohmann::basic_json<> item) {
        if (!isRelated(item)) {
            throw std::runtime_error("Invalid JSON for Ship deserialization");
        }

        MatchSettings matchSettings(
            item["shipsDictionary"].get<std::map<int, int>>(),
            item["fieldSize"].get<int>()
        );
        matchSettings.toggleDoubleDamage(item["isDoubleDamage"].get<bool>());

        if (matchSettings.getDamageCount() != item["damageCount"].get<int>()) {
            throw std::runtime_error("Damage count mismatch during deserialization");
        }

        return matchSettings;
    }
} // namespace cpp_warships::application