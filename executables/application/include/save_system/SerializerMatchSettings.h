#pragma once

#include "Serializer.h"
#include "../GameStateDTO.h"

namespace cpp_warships::application {

    class SerializerMatchSettings final : public Serializer {
    public:
        json serialize(const GameStateDTO &object) override;
        void deserialize(const json &j, GameStateDTO &object) override;
    };
} // namespace cpp_warships::application