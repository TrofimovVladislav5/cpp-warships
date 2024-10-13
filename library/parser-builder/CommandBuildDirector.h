#pragma once

#include "ParserCommandBuilder.h"

class CommandBuildDirector {
private:
    ParserCommandBuilder &commandBuilder;
public:
    explicit CommandBuildDirector(ParserCommandBuilder *commandBuilder);

    ParserCommandBuilder& buildBasicCommand(ParseCallback function, std::string description) const;
    void reset() const;
};
