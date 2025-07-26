#pragma once

#include "../../model/save/Serializer.h"

namespace cpp_warships::application {

    class SerializerManager final : public Serializer {
    public:
        json serialize(const GameStateDTO &object) override;
        void deserialize(const json &j, GameStateDTO &object) override;
    };
} // namespace cpp_warships::application
