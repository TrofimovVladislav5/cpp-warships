#pragma once

#include "Serializer.h"

namespace cpp_warships::application {

    class SerializerFactory {
    public:
        virtual ~SerializerFactory() = default;
        virtual Serializer* createSerializer() = 0;
    };

    template <typename SerializerType>
    class SpecificSerializerFactory final : public SerializerFactory {
    public:
        Serializer* createSerializer() override { return new SerializerType(); }
    };
} // namespace cpp_warships::application