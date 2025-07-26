#pragma once

#include "../../model/save/Serializer.h"

namespace cpp_warships::application {

    class SerializerSkillManager final : public Serializer {
        json serialize(const GameStateDTO &object) override;
        void deserialize(const json &j, GameStateDTO &object) override;
    };
} // namespace cpp_warships::application