#include "../../include/save_system/SegmentJsonSerializer.h"

namespace cpp_warships::application {

    bool SegmentJsonSerializer::isRelated(nlohmann::basic_json<> item) {
        return item.contains("authenticator") &&
               item["authenticator"].is_string() &&
               item["authenticator"] == "segment";
    }

    nlohmann::json SegmentJsonSerializer::serialize(Segment& item) {
        nlohmann::json finalJson{
            {"authenticator", "segment"},
            {"maxSegmentHealth", item.getMaxSegmentHealth()},
            {"currentHealth", item.getHitPoints()}
        };

        return finalJson;
    }

    Segment SegmentJsonSerializer::deserialize(nlohmann::basic_json<> item) {
        if (!isRelated(item)) {
            throw std::invalid_argument("Item is not related to Segment");
        }

        return Segment{
            item["maxSegmentHealth"],
            item["currentHealth"]
        };
    }

} // namespace cpp_warships::application