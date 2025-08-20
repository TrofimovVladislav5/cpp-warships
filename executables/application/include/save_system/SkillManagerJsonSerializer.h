#pragma once

#include <cpp_warships/game_saves/include/ISerializer.h>
#include <nlohmann/json.hpp>

#include "../player_skills/SkillManager.h"
#include "GameFieldJsonSerializer.h"
#include "MatchSettingsJsonSerializer.h"

namespace cpp_warships::application {

    class SkillManagerJsonSerializer
        : public game_saves::ISerializer<nlohmann::json, SkillManager, SkillManagerName, GameFieldJsonSerializer, MatchSettingsJsonSerializer>
    {
    public:
        ~SkillManagerJsonSerializer() override = default;

        bool isRelated(nlohmann::basic_json<> item) override;
        nlohmann::json serialize(SkillManager& item) override;
        SkillManager deserialize(nlohmann::basic_json<> item) override;
    };

} // namespace cpp_warships::application