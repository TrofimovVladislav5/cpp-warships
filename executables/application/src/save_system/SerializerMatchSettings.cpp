#include "../../include/save_system/SerializerMatchSettings.h"

namespace cpp_warships::application {

    json SerializerMatchSettings::serialize(const GameStateDTO &object) {
        json j = json{{
            "isActiveDoubleDamage", object.settings->getIsDoubleDamage()
        }};
        return j;
    }

    void SerializerMatchSettings::deserialize(const json &j, GameStateDTO &object) {
        if (object.shipsSizes.empty()) {
            object.settings = nullptr;
            return;
        }
        object.settings = new MatchSettings(object.shipsSizes, object.fieldSize);
        if (j.contains("isActiveDoubleDamage")) {
            object.settings->toggleDoubleDamage(j.at("isActiveDoubleDamage").get<bool>());
        }
    }
} // namespace cpp_warships::application