#pragma once

#include <application/core/Segment.h>
#include <serialization/ISerializer.h>

#include <nlohmann/json.hpp>

namespace cpp_warships::persistence::serializers {
    inline char SEGMENT_SERIALIZER_NAME[] = "Segment";

    /** @brief Writes a segment as its maximum and current health. */
    class SegmentJsonSerializer final
        : public serialization::
              ISerializer<nlohmann::json, core::Segment, SEGMENT_SERIALIZER_NAME> {
    public:
        bool isRelated(nlohmann::json item) override;
        nlohmann::json serialize(core::Segment& item) override;
        core::Segment deserialize(nlohmann::json item) override;
    };
}  // namespace cpp_warships::persistence::serializers
