#include "ParameterBuildDirector.h"

ParameterBuildDirector::ParameterBuildDirector(ParserParameterBuilder* parameterBuilder)
    : parameterBuilder(*parameterBuilder)
{}

ParserParameterBuilder& ParameterBuildDirector::buildNecessary(std::string flag, std::regex validator) {
    parameterBuilder.addFlag(std::move(flag));
    parameterBuilder.setNecessary(true);
    parameterBuilder.setValidator(std::move(validator));

    return parameterBuilder;
}

ParserParameterBuilder& ParameterBuildDirector::buildUnnecessary(std::string flag, std::regex validator) {
    parameterBuilder.addFlag(std::move(flag));
    parameterBuilder.setNecessary(false);
    parameterBuilder.setValidator(std::move(validator));

    return parameterBuilder;
}

void ParameterBuildDirector::reset() const {
    parameterBuilder.reset();
}
