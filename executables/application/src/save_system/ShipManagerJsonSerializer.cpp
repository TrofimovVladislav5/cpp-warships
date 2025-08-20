#include "../../include/save_system/ShipManagerJsonSerializer.h"

#include <nlohmann/json.hpp>

namespace cpp_warships::application {

    bool ShipManagerJsonSerializer::isRelated(nlohmann::basic_json<> item) {
        return item.contains("authenticator") &&
               item["authenticator"].is_string() &&
               item["authenticator"] == "shipManager";
    }

    nlohmann::json ShipManagerJsonSerializer::serialize(ShipManager& item) {
        return nlohmann::json{
            {"authenticator", "shipManager"},
            {"shipsSizes", item.getShipsSizes()},
            {"ships", serializeShips(item.getShips())}
        };
    }

    ShipManager ShipManagerJsonSerializer::deserialize(nlohmann::basic_json<> item) {
        return ShipManager{
            item["shipsSizes"].get<std::map<int, int>>(),
            deserializeShips(item["ships"])
        };
    }

    nlohmann::json ShipManagerJsonSerializer::serializeShips(const std::vector<Ship *>& ships) const {
        nlohmann::json finalJson = nlohmann::json::array();
        auto shipSerializer = std::get<0>(childrenSerializers);

        for (const auto& ship : ships) {
            auto shipJson = shipSerializer.serialize(*ship);
            finalJson.emplace_back(shipJson);
        }

        return finalJson;
    }

    std::vector<Ship*> ShipManagerJsonSerializer::deserializeShips(const nlohmann::basic_json<>& item) {
        if (!item.is_array()) {
            throw std::runtime_error("Invalid ShipManager JSON data: 'ships' should be an array");
        }

        std::vector<Ship*> finalJson{};
        auto shipSerializer = std::get<0>(childrenSerializers);

        for (const auto& shipJson : item) {
            auto* shipValue = new Ship(std::get<0>(childrenSerializers).deserialize(shipJson));

            finalJson.push_back(shipValue);
        }

        return finalJson;
    }
} // namespace cpp_warships::application