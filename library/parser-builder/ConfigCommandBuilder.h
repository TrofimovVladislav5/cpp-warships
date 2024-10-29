#pragma once
#include "ShipPlacementException.h"
#include "ParserCommandBuilder.h"

class ConfigCommandBuilder: public ParserCommandBuilder {
public:
    ~ConfigCommandBuilder() override = default;

    ConfigCommandBuilder& setDescription(std::string description) override;
    ConfigCommandBuilder& addParameter(ParserParameter parameter) override;
    ConfigCommandBuilder& setDisplayError(ParseCallback displayError) override;
    ConfigCommandBuilder& setCallback(ParseCallback function) override;
    ConfigCommandBuilder& setResolveAllFlags(bool resolveAll) override;
    ConfigCommandBuilder& setPrintHelp(ParseCallback help) override;
    ParserCommandInfoConfig buildAndReset();
    ParserCommandInfoConfig build();
};
