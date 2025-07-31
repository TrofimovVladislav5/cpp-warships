#include "../../include/save_system/SerializerGameField.h"

namespace cpp_warships::application {

    json SerializerGameField::unorderedSetToJson(const std::unordered_set<std::pair<int, int>, hashFunc>& attacksOnField) {
        json attacksArray = json::array();
        for (const auto& pair : attacksOnField) {
            attacksArray.push_back({
                {"first", pair.first},
                {"second", pair.second}
            });
        }
        return attacksArray;
    }

    std::unordered_set<std::pair<int, int>, hashFunc> SerializerGameField::unorderedSetFromJson(const json& j) {
        std::unordered_set<std::pair<int, int>, hashFunc> attacksOnFields;
        for (const auto& pair : j) {
            int first = pair.at("first").get<int>();
            int second = pair.at("second").get<int>();
            attacksOnFields.emplace(first, second);
        }
        return attacksOnFields;
    }



    json SerializerGameField::unorderedMapToJson(
        const std::unordered_map<Ship*, std::unordered_set<std::pair<int, int>, hashFunc>>& shipsCoordinatesMap
    )
    {
        json shipsMap = json::array();
        for (const auto& [ship, coordinates] : shipsCoordinatesMap) {
            json shipJson = shipToJson(*ship);
            json coordinatesJson = unorderedSetToJson(coordinates);
            shipsMap.push_back({
                {"ship", shipJson},
                {"coordinates", coordinatesJson}
            });
        }
        return shipsMap;
    }

    std::unordered_map<Ship*, std::unordered_set<std::pair<int, int>, hashFunc> > SerializerGameField::unorderedMapFromJson(const json &j) {
        std::unordered_map<Ship*, std::unordered_set<std::pair<int, int>, hashFunc>> shipsCoordinateMap;
        for (const auto& element : j) {
            Ship* ship = shipFromJson(element.at("ship"));
            std::unordered_set<std::pair<int, int>, hashFunc> coordinates = unorderedSetFromJson(element.at("coordinates"));
            shipsCoordinateMap.emplace(ship, coordinates);
        }
        return shipsCoordinateMap;
    }


    json SerializerGameField::serialize(const GameStateDTO &object) {
        json playerFieldShips = unorderedMapToJson(object.playerField->getShipsCoordinateMap());
        json playerFieldAttacks = unorderedSetToJson(object.playerField->getAttacksOnField());
        json enemyFieldShips = unorderedMapToJson(object.enemyField->getShipsCoordinateMap());
        json enemyFieldAttacks = unorderedSetToJson(object.enemyField->getAttacksOnField());

        json j = {
            {"width", object.playerField->getHeight()},
            {"height", object.playerField->getWidth()},
            {"shipsCoordinatesMap", playerFieldShips},
            {"attacksOnField", playerFieldAttacks},
            {"enemyShipsCoordinatesMap", enemyFieldShips},
            {"enemyAttacksOnField", enemyFieldAttacks}
        };
        return j;
    }

    void SerializerGameField::deserialize(const json &j, GameStateDTO &object) {
        auto attacksOnField = unorderedSetFromJson(j.at("attacksOnField"));
        auto enemyAttacksOnField = unorderedSetFromJson(j.at("enemyAttacksOnField"));
        auto shipsCoordinateMap = unorderedMapFromJson(j.at("shipsCoordinatesMap"));
        auto enemyShipsCoordinateMap = unorderedMapFromJson(j.at("enemyShipsCoordinatesMap"));
        int w = j.at("width");
        int h = j.at("height");
        GameField* playerField = new GameField(w, h, shipsCoordinateMap, attacksOnField);
        GameField* enemyField = new GameField(w, h, enemyShipsCoordinateMap, enemyAttacksOnField);
        object.playerField = playerField;
        object.enemyField = enemyField;
    }
} // namespace cpp_warships::application