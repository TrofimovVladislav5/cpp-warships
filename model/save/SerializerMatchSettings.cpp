#include "SerializerMatchSettings.h"

json SerializerMatchSettings::serialize(const GameStateDTO &object) {
    json j = json {
        {"damageCount", object.settings->damageCount},
        {"isActiveDoubleDamage", object.settings->isActiveDoubleDamage}
    };
    return j;
}

void SerializerMatchSettings::deserialize(const json &j, GameStateDTO &object) {
    if (object.shipsSizes.empty()) {
        object.settings = nullptr;
        return;
    }
    object.settings = new MatchSettings(object.shipsSizes, object.fieldSize);
    if (j.contains("damageCount")) object.settings->damageCount = j.at("damageCount").get<int>();
    if (j.contains("isActiveDoubleDamage")) object.settings->isActiveDoubleDamage = j.at("isActiveDoubleDamage").get<bool>();
}
