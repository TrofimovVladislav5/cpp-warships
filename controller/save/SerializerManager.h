#pragma once
#include "save/Serializer.h"

class SerializerManager final : public Serializer {
public:
    json serialize(const GameStateDTO &object) override;
    void deserialize(const json &j, GameStateDTO &object) override;
};



