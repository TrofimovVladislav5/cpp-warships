#pragma once

#include <cpp_warships/game_saves/include/ISerializer.h>
#include <nlohmann/json.hpp>

#include "../ShipManager.h"
#include "ShipJsonSerializer.h"

namespace cpp_warships::application {

    class ShipManagerJsonSerializer
        : public game_saves::ISerializer<nlohmann::json, ShipManager, ShipManagerName, ShipJsonSerializer>
    {
    private:
        [[nodiscard]] nlohmann::json serializeShips(const std::vector<Ship *>& ships) const;
        std::vector<Ship*> deserializeShips(const nlohmann::basic_json<>& item);
    public:
        ~ShipManagerJsonSerializer() override = default;

        bool isRelated(nlohmann::basic_json<> item) override;
        nlohmann::json serialize(ShipManager& item) override;
        ShipManager deserialize(nlohmann::basic_json<> item) override;
    };

} // namespace cpp_warships::application