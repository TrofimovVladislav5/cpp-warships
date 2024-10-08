#pragma once
#include "ParserParameterBuilder.h"
#include "library/parser/ParserParameter.h"

class DefaultParameterBuilder: public ParserParameterBuilder {
public:
    ~DefaultParameterBuilder() override = default;

    DefaultParameterBuilder& addFlag(std::string flag) override;
    DefaultParameterBuilder& setValidator(std::regex validator) override;
    DefaultParameterBuilder& setDescription(std::string description) override;
    DefaultParameterBuilder& setNecessary(bool necessary) override;

    ParserParameter build();
    ParserParameter buildAndReset();
};
