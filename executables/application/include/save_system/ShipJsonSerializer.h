#pragma once

#include <cpp_warships/game_saves/include/ISerializer.h>
#include <nlohmann/json.hpp>

#include "SegmentJsonSerializer.h"
#include "../Ship.h"

namespace cpp_warships::application {

    class ShipJsonSerializer
        : public game_saves::ISerializer<nlohmann::json, Ship, ShipName, SegmentJsonSerializer>
    {
    private:
        [[nodiscard]] nlohmann::json serializeSegments(const std::vector<Segment*>& segments) const;
        [[nodiscard]] std::vector<Segment*> deserializeSegments(const nlohmann::json& json) const;
    public:
        bool isRelated(nlohmann::basic_json<> item) override;
        nlohmann::json serialize(Ship& item) override;
        Ship deserialize(nlohmann::basic_json<> item) override;
    };

} // namespace cpp_warships::application