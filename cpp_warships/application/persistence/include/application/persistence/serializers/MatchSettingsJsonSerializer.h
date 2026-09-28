#pragma once

#include <application/core/MatchSettings.h>
#include <serialization/ISerializer.h>

#include <nlohmann/json.hpp>

namespace cpp_warships::persistence {
    inline char MATCH_SETTINGS_SERIALIZER_NAME[] = "MatchSettings";

    /** @brief Writes the rules a match is played under, fleet composition
     * included.
     */
    class MatchSettingsJsonSerializer final
        : public serialization::
              ISerializer<nlohmann::json, core::MatchSettings, MATCH_SETTINGS_SERIALIZER_NAME> {
       public:
        bool isRelated(nlohmann::json item) override;
        nlohmann::json serialize(core::MatchSettings& item) override;
        core::MatchSettings deserialize(nlohmann::json item) override;
    };
}  // namespace cpp_warships::persistence
