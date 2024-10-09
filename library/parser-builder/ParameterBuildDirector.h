#pragma once
#include "ParserParameterBuilder.h"

class ParameterBuildDirector {
private:
    ParserParameterBuilder& parameterBuilder;
public:
    explicit ParameterBuildDirector(ParserParameterBuilder *parameterBuilder);
    ParserParameterBuilder& buildNecessary(std::string flag, std::regex validator = std::regex("."));
    ParserParameterBuilder& buildUnnecessary(std::string flag, std::regex validator = std::regex("."));

    void reset() const;
};
