#include <application/persistence/serializers/BoardJsonSerializer.h>
#include <serialization/exceptions/DeserializationException.h>

#include <unordered_set>
#include <utility>
#include <vector>

namespace cpp_warships::persistence::serializers {
    bool BoardJsonSerializer::isRelated(nlohmann::json item) {
        return item.contains("width") && item.contains("height") && item.contains("ships") &&
               item.contains("attackedCells");
    }

    nlohmann::json BoardJsonSerializer::serialize(core::Board& item) {
        ShipJsonSerializer shipSerializer = std::get<0>(childrenSerializers);

        nlohmann::json ships = nlohmann::json::array();
        for (core::Ship ship : item.ships()) {
            ships.push_back(shipSerializer.serialize(ship));
        }

        nlohmann::json attackedCells = nlohmann::json::array();
        for (const core::Coordinate& cell : item.attackedCells()) {
            attackedCells.push_back({{"x", cell.x}, {"y", cell.y}});
        }

        return nlohmann::json{
            {"width", item.width()},
            {"height", item.height()},
            {"ships", ships},
            {"attackedCells", attackedCells}
        };
    }

    core::Board BoardJsonSerializer::deserialize(nlohmann::json item) {
        if (!isRelated(item)) {
            throw serialization::exceptions::DeserializationException(
                "Board",
                "JSON does not describe a board"
            );
        }

        ShipJsonSerializer shipSerializer = std::get<0>(childrenSerializers);
        std::vector<core::Ship> ships;
        for (const nlohmann::json& ship : item["ships"]) {
            ships.push_back(shipSerializer.deserialize(ship));
        }

        std::unordered_set<core::Coordinate> attackedCells;
        for (const nlohmann::json& cell : item["attackedCells"]) {
            attackedCells.insert({cell["x"].get<int>(), cell["y"].get<int>()});
        }

        return core::Board{
            item["width"].get<int>(),
            item["height"].get<int>(),
            std::move(ships),
            std::move(attackedCells)
        };
    }
}  // namespace cpp_warships::persistence::serializers
