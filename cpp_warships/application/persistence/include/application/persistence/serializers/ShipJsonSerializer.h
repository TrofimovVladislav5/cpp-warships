#pragma once

#include <application/core/Ship.h>
#include <application/persistence/serializers/SegmentJsonSerializer.h>
#include <serialization/ISerializer.h>

#include <nlohmann/json.hpp>

namespace cpp_warships::persistence {
    inline char SHIP_SERIALIZER_NAME[] = "Ship";

    /** @brief Writes a ship as its origin, orientation and segments.
     *  Coordinates are derived on load, so only the origin needs storing. */
    class ShipJsonSerializer final
        : public serialization::
              ISerializer<nlohmann::json, core::Ship, SHIP_SERIALIZER_NAME, SegmentJsonSerializer> {
       public:
        bool isRelated(nlohmann::json item) override;
        nlohmann::json serialize(core::Ship& item) override;
        core::Ship deserialize(nlohmann::json item) override;
    };
}  // namespace cpp_warships::persistence
