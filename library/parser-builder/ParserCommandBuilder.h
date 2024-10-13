#pragma once
#include "library/parser/ParserCommandInfo.h"

class ParserCommandBuilder {
protected:
    std::string description;
    std::vector<ParserParameter> parameters;
    ParseCallback executable;
    ParseCallback displayError;
public:
    virtual ~ParserCommandBuilder() = default;

    virtual ParserCommandBuilder& setDescription(std::string description) = 0;
    virtual ParserCommandBuilder& addParameter(ParserParameter parameter) = 0;
    virtual ParserCommandBuilder& setDisplayError(ParseCallback displayError) = 0;
    virtual ParserCommandBuilder& setCallback(ParseCallback function) = 0;
    void reset();
};
