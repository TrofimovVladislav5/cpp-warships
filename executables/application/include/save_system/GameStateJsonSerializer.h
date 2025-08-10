#pragma once

#include <cpp_warships/game_saves/include/ISerializer.h>

#include <nlohmann/json.hpp>

#include "../GameStateDTO.h"
#include "GameFieldJsonSerializer.h"
#include "ShipManagerJsonSerializer.h"
#include "SkillManagerJsonSerializer.h"

namespace cpp_warships::application {

    class GameStateJsonSerializer
        : public game_saves::ISerializer<nlohmann::json, GameStateDTO, GameStateDTOName, ShipManagerJsonSerializer, GameFieldJsonSerializer, SkillManagerJsonSerializer, MatchSettingsJsonSerializer>
    {
    public:
        ~GameStateJsonSerializer() override = default;

        bool isRelated(nlohmann::basic_json<> item) override;
        nlohmann::json serialize(GameStateDTO& item) override;
        GameStateDTO deserialize(nlohmann::basic_json<> item) override;
    };
} // namespace cpp_warships::application