#include "DefaultParameterBuilder.h"

#include "library/parser/ParserParameter.h"

DefaultParameterBuilder& DefaultParameterBuilder::addFlag(std::string flag) {
    this->flags.push_back(flag);
    return *this;
}

DefaultParameterBuilder& DefaultParameterBuilder::setValidator(std::regex validator) {
    this->validator = validator;
    return *this;
}

DefaultParameterBuilder& DefaultParameterBuilder::setDescription(std::string description) {
    this->description = description;
    return *this;
}

DefaultParameterBuilder& DefaultParameterBuilder::setNecessary(bool necessary) {
    this->necessary = necessary;
    return *this;
}

ParserParameter DefaultParameterBuilder::build() {
    return ParserParameter({
        flags,
        validator,
        description,
        necessary
    });
}

ParserParameter DefaultParameterBuilder::buildAndReset() {
    ParserParameter parameter = this->build();
    this->reset();

    return parameter;
}
