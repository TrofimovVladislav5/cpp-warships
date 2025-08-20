#include "../../include/save_system/GameStateJsonSerializer.h"

namespace cpp_warships::application {

    bool GameStateJsonSerializer::isRelated(nlohmann::basic_json<> item) {
        return item.contains("authenticator") &&
               item["authenticator"].is_string() &&
               item["authenticator"] == "gameState";
    }

    nlohmann::json GameStateJsonSerializer::serialize(GameStateDTO& item) {
        auto shipManagerSerializer = std::get<0>(childrenSerializers);
        auto fieldSerializer = std::get<1>(childrenSerializers);
        auto skillManagerSerializer = std::get<2>(childrenSerializers);
        auto matchSettingsSerializer = std::get<3>(childrenSerializers);

        return nlohmann::json{
            {"authenticator", "gameState"},
            {"playerShipManager", shipManagerSerializer.serialize(*item.playerManager)},
            {"playerField", fieldSerializer.serialize(*item.playerField)},
            {"enemyShipManager", shipManagerSerializer.serialize(*item.enemyManager)},
            {"enemyField", fieldSerializer.serialize(*item.enemyField)},
            {"roundNumber", item.roundNumber},
            {"shipsSizes", item.shipsSizes},
            {"skillManager", skillManagerSerializer.serialize(*item.playerSkillManager)},
            {"settings", matchSettingsSerializer.serialize(*item.settings)}
        };
    }

    GameStateDTO GameStateJsonSerializer::deserialize(nlohmann::basic_json<> item) {
        if (!isRelated(item)) {
            throw std::runtime_error("Invalid JSON for Ship deserialization");
        }

        auto shipManagerSerializer = std::get<0>(childrenSerializers);
        auto fieldSerializer = std::get<1>(childrenSerializers);
        auto skillManagerSerializer = std::get<2>(childrenSerializers);
        auto matchSettingsSerializer = std::get<3>(childrenSerializers);
        GameStateDTO gameState{};

        gameState.playerField = new GameField(fieldSerializer.deserialize(item["playerField"]));
        gameState.playerManager = new ShipManager(shipManagerSerializer.deserialize(item["playerShipManager"]));
        gameState.enemyField = new GameField(fieldSerializer.deserialize(item["enemyField"]));
        gameState.enemyManager = new ShipManager(shipManagerSerializer.deserialize(item["enemyShipManager"]));
        gameState.playerSkillManager = new SkillManager(skillManagerSerializer.deserialize(item["skillManager"]));
        gameState.settings = new MatchSettings(matchSettingsSerializer.deserialize(item["settings"]));
        gameState.roundNumber = item["roundNumber"].get<int>();
        gameState.shipsSizes = item["shipsSizes"].get<std::map<int, int>>();


        return gameState;
    }
} // namespace cpp_warships::application