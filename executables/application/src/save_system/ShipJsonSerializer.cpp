#include "../../include/save_system/ShipJsonSerializer.h"

namespace cpp_warships::application {

    bool ShipJsonSerializer::isRelated(nlohmann::basic_json<> item) {
        return item.contains("authenticator") &&
               item["authenticator"].is_string() &&
               item["authenticator"] == "ship";
    }

    nlohmann::json ShipJsonSerializer::serialize(Ship& item) {
        nlohmann::json finalJson{
            {"authenticator", "ship"},
            {"maxSegmentHealth", item.getMaxSegmentHealth()},
            {"length", item.getLength()},
        };
        finalJson["segments"] = serializeSegments(item.getSegments());

        return finalJson;
    }

    Ship ShipJsonSerializer::deserialize(nlohmann::basic_json<> item) {
        if (!isRelated(item)) {
            throw std::invalid_argument("Item is not related to Ship");
        }

        return Ship{ deserializeSegments(item["segments"]) };
    }

    std::vector<Segment*> ShipJsonSerializer::deserializeSegments(const nlohmann::json& json) const {
        std::vector<Segment*> segments;
        auto serializer = get<0>(childrenSerializers);

        for (const auto& segmentJson : json) {
            Segment deserializedSegment = serializer.deserialize(segmentJson);
            auto* segment = new Segment(deserializedSegment);
            segments.push_back(segment);
        }

        return segments;
    }

    nlohmann::json ShipJsonSerializer::serializeSegments(const std::vector<Segment *> &segments) const {
        nlohmann::json finalJson = nlohmann::json::array();
        auto serializer = get<0>(childrenSerializers);

        for (Segment *segment : segments) {
            finalJson.push_back(serializer.serialize(*segment));
        }

        return finalJson;
    }
} // namespace cpp_warships::application