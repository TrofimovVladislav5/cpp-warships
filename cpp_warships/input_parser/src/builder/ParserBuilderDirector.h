#pragma once
#include "ConfigCommandBuilder.h"

template <typename T>
class ParserBuilderDirector {
private:
    ParserCommandBuilder<T>& commandBuilder;
public:
    explicit ParserBuilderDirector(ParserCommandBuilder<T> &commandBuilder)
        : commandBuilder(commandBuilder)
    {}
};
