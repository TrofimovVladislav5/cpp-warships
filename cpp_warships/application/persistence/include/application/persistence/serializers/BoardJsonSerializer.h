#pragma once

#include <application/core/Board.h>
#include <application/persistence/serializers/ShipJsonSerializer.h>
#include <serialization/ISerializer.h>

#include <nlohmann/json.hpp>

namespace cpp_warships::persistence::serializers {
    inline char BOARD_SERIALIZER_NAME[] = "Board";

    /** @brief Writes a board as its size, its ships and the cells attacked so far. */
    class BoardJsonSerializer final
        : public serialization::
              ISerializer<nlohmann::json, core::Board, BOARD_SERIALIZER_NAME, ShipJsonSerializer> {
    public:
        bool isRelated(nlohmann::json item) override;
        nlohmann::json serialize(core::Board& item) override;
        core::Board deserialize(nlohmann::json item) override;
    };
}  // namespace cpp_warships::persistence::serializers
