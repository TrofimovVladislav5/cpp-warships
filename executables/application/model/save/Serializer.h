#pragma once

#include <nlohmann/json.hpp>

#include "../game/GameStateDTO.h"

namespace cpp_warships::application {

    using json = nlohmann::json;

    class Serializer {
    protected:
        json segmentToJson(const Segment& s);
        json shipToJson(const Ship& ship);
        json shipsSizesToJson(const std::map<int, int>& shipsSizes);
        json segmentsToJson(const std::vector<Segment*>& segments);
        json vectorShipsToJson(const std::vector<Ship*>& ships);
        [[nodiscard]] Segment* segmentFromJson(const json& j);
        [[nodiscard]] Ship* shipFromJson(const json& j);
        [[nodiscard]] std::vector<Segment*> segmentsFromJson(const json& j);
        [[nodiscard]] std::vector<Ship*> vectorShipsFromJson(const json& j);
        [[nodiscard]] std::map<int, int> shipSizesFromJson(const json& j);
    public:
        virtual ~Serializer() = default;
        virtual json serialize(const GameStateDTO& object) = 0;
        virtual void deserialize(const json& j, GameStateDTO& object) = 0;
    };
} // namespace cpp_warships::application