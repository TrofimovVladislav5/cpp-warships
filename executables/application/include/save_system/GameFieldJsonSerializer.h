#pragma once

#include <cpp_warships/game_saves/include/ISerializer.h>

#include <nlohmann/json.hpp>

#include "ShipJsonSerializer.h"
#include "../GameField.h"

namespace cpp_warships::application {

    class GameFieldJsonSerializer
        : public game_saves::ISerializer<nlohmann::json, GameField, GameFieldName, ShipJsonSerializer>
    {
    private:
        [[nodiscard]] nlohmann::json serializeShipsMap(const ShipsField& field) const;
        [[nodiscard]] ShipsField deserializeShipsMap(const nlohmann::json& json) const;
    public:
        bool isRelated(nlohmann::basic_json<> item) override;
        nlohmann::json serialize(GameField& item) override;
        GameField deserialize(nlohmann::basic_json<> item) override;
    };

} // namespace cpp_warships::application