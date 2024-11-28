#pragma once
#include "Serializer.h"

class SerializerFactory {
public:
    virtual ~SerializerFactory() = default;
    virtual Serializer* createSerializer() = 0;
};

template <typename SerializerType>
class SpecificSerializerFactory final : public SerializerFactory {
public:
    Serializer* createSerializer() override {
        return new SerializerType();
    }
};

