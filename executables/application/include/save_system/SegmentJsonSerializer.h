#pragma once

#include <cpp_warships/game_saves/include/ISerializer.h>
#include <nlohmann/json.hpp>

#include "../Segment.h"

namespace cpp_warships::application {

    class SegmentJsonSerializer
        : public game_saves::ISerializer<nlohmann::json, Segment, SegmentName>
    {
    public:
        bool isRelated(nlohmann::basic_json<> item) override;
        nlohmann::json serialize(Segment& item) override;
        Segment deserialize(nlohmann::basic_json<> item) override;
    };

} // namespace cpp_warships::application