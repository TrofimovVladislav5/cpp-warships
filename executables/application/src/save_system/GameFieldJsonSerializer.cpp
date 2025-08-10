#include "../../include/save_system/GameFieldJsonSerializer.h"

#include <nlohmann/json.hpp>

namespace cpp_warships::application {

    bool GameFieldJsonSerializer::isRelated(nlohmann::basic_json<> item) {
        return item.contains("authenticator") &&
            item["authenticator"].is_string() &&
            item["authenticator"] == "gameField";
    }

    nlohmann::json GameFieldJsonSerializer::serialize(GameField& item) {
        return nlohmann::json{
            {"authenticator", "gameField"},
            {"width", item.getWidth()},
            {"height", item.getHeight()},
            {"attacksOnField", item.getAttacksOnField()},
            {"shipsCoordinatesMap", serializeShipsMap(item.getShipsCoordinateMap())},
        };
    }

    GameField GameFieldJsonSerializer::deserialize(nlohmann::basic_json<> item) {
        if (!isRelated(item)) {
            throw std::runtime_error("Invalid GameField JSON data");
        }

        return GameField{
            item["width"].get<int>(),
            item["width"].get<int>(),
            deserializeShipsMap(item["shipsCoordinatesMap"]),
            item["attacksOnField"]
        };
    }

    nlohmann::json GameFieldJsonSerializer::serializeShipsMap(const ShipsField& field) const {
        nlohmann::json shipsMapJson;
        auto shipSerializer = std::get<0>(childrenSerializers);

        for (const auto& [ship, coordinates] : field) {
            nlohmann::json shipJson;
            shipJson["ship"] = shipSerializer.serialize(*ship);
            shipJson["coordinates"] = coordinates;

            shipsMapJson.push_back(shipJson);
        }
        return shipsMapJson;
    }

    ShipsField GameFieldJsonSerializer::deserializeShipsMap(const nlohmann::json& json) const {
        ShipsField field;
        auto shipSerializer = std::get<0>(childrenSerializers);

        for (const auto& shipJson : json) {
            auto* shipValue = new Ship(shipSerializer.deserialize(shipJson["ship"]));
            auto shipCoordinates = shipJson["coordinates"].get<ShipCoordinatesSet>();

            field.emplace(
                shipValue,
                shipCoordinates
            );
        }

        return field;
    }
} // namespace cpp_warships::application