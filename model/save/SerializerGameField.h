#pragma once
#include "Serializer.h"


class SerializerGameField final : public Serializer {
private:
    json unorderedSetToJson(const std::unordered_set<std::pair<int, int>, hashFunc>& attacksOnField);
    [[nodiscard]] std::unordered_set<std::pair<int, int>, hashFunc> unorderedSetFromJson(const json& j);
    json unorderedMapToJson(const std::unordered_map<Ship*, std::unordered_set<std::pair<int, int>, hashFunc>>& shipsCoordinatesMap);
    [[nodiscard]] std::unordered_map<Ship*, std::unordered_set<std::pair<int, int>, hashFunc>> unorderedMapFromJson(const json& j);

public:
    json serialize(const GameStateDTO &object) override;
    void deserialize(const json &j, GameStateDTO &object) override;
};
