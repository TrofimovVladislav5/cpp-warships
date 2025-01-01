#include "Serializer.h"

json Serializer::segmentToJson(const Segment& s) {
    return json{{"maxSegmentHealth", s.getMaxSegmentHealth()},
                {"currentSegmentHealth", s.getHitPoints()}};
}

json Serializer::shipToJson(const Ship& ship) {
    return json{{"maxSegmentHealth", ship.getMaxSegmentHealth()},
                {"segments", segmentsToJson(ship.getSegments())},
                {"length", ship.getLength()}};
}

json Serializer::shipsSizesToJson(const std::map<int, int>& shipsSizes) {
    json j;
    for (const auto& [size, count] : shipsSizes) {
        j[std::to_string(size)] = count;
    }
    return j;
}

json Serializer::segmentsToJson(const std::vector<Segment*>& segments) {
    json result = json::array();
    for (const auto& segment : segments) {
        result.push_back(segmentToJson(*segment));
    }
    return result;
}

json Serializer::vectorShipsToJson(const std::vector<Ship*>& ships) {
    json shipsJson = json::array();
    for (const auto& ship : ships) {
        shipsJson.push_back(shipToJson(*ship));
    }
    return shipsJson;
}

Segment* Serializer::segmentFromJson(const json& j) {
    int maxSegmentHealth = j.at("maxSegmentHealth").get<int>();
    int currentSegmentHealth = j.at("currentSegmentHealth").get<int>();
    return new Segment(maxSegmentHealth, currentSegmentHealth);
}
Ship* Serializer::shipFromJson(const json& j) {
    std::vector<Segment*> segments = segmentsFromJson(j.at("segments"));
    return new Ship(segments);
}

std::vector<Segment*> Serializer::segmentsFromJson(const json& j) {
    std::vector<Segment*> result;
    for (const auto& segmentJson : j) {
        result.push_back(segmentFromJson(segmentJson));
    }
    return result;
}

std::vector<Ship*> Serializer::vectorShipsFromJson(const json& j) {
    std::vector<Ship*> result;
    for (const auto& shipJson : j) {
        result.push_back(shipFromJson(shipJson));
    }
    return result;
}

std::map<int, int> Serializer::shipSizesFromJson(const json& j) {
    std::map<int, int> result;
    for (const auto& [key, value] : j.items()) {
        result[std::stoi(key)] = value.get<int>();
    }
    return result;
}
