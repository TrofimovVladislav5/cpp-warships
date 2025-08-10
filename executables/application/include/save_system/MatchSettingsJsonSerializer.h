#pragma once

#include <cpp_warships/game_saves/include/ISerializer.h>
#include <nlohmann/json.hpp>

#include "../MatchSettings.h"

namespace cpp_warships::application {

    class MatchSettingsJsonSerializer
        : public game_saves::ISerializer<nlohmann::json, MatchSettings, MatchSettingsName>
    {
    public:
        ~MatchSettingsJsonSerializer() override = default;

        bool isRelated(nlohmann::basic_json<> item) override;
        nlohmann::json serialize(MatchSettings& item) override;
        MatchSettings deserialize(nlohmann::basic_json<> item) override;
    };

} // namespace cpp_warships::application