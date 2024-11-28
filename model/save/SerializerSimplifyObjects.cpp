#include "SerializerSimplifyObjects.h"


json SerializerSimplifyObjects::serialize(const GameStateDTO &object) {
    json j;
    j["fieldSize"] = object.fieldSize;
    j["roundNumber"] = object.roundNumber;
    j["currentShips"] = object.currentShips;
    for (const auto& [key, value] : object.shipsSizes) {
        j["shipsSizes"][std::to_string(key)] = value;
    }
    return j;
}

void SerializerSimplifyObjects::deserialize(const json &j, GameStateDTO &object) {
    object.fieldSize = j["fieldSize"];
    object.roundNumber = j["roundNumber"];
    object.currentShips = j["currentShips"];
    for (const auto& [key, value] : j["shipsSizes"].items()) {
        object.shipsSizes[std::stoi(key)] = value;
    }
}