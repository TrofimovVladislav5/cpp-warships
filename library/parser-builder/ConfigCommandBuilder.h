#pragma once
#include "ParserCommandBuilder.h"

class ConfigCommandBuilder: public ParserCommandBuilder {
public:
    ~ConfigCommandBuilder() override = default;

    ConfigCommandBuilder& setDescription(std::string description) override;
    ConfigCommandBuilder& addParameter(ParserParameter parameter) override;
    ConfigCommandBuilder& setDisplayError(ParseCallback displayError) override;
    ConfigCommandBuilder& setCallback(ParseCallback function) override;
    ParserCommandInfoConfig buildAndReset();
    ParserCommandInfoConfig build();
};
