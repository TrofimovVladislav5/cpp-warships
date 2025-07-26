#pragma once
#include "Serializer.h"

class SerializerSimplifyObjects final : public Serializer {
public:
    json serialize(const GameStateDTO &object) override;
    void deserialize(const json &j, GameStateDTO &object) override;
};
