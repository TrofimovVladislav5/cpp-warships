#include "ParserParameter.h"

#include <iostream>

// template<typename T>
ParserParameter::ParserParameter(std::vector<std::string> flags, std::regex validator)
    : validator(validator)
    , flags(std::move(flags))
{}

// template<typename T>
std::string ParserParameter::getDescription() {
    return this->description;
}

// template<typename T>
ParserParameter::ParserParameter(std::vector<std::string> flags, std::regex validator, std::string description)
    : validator(validator)
    , description(std::move(description))
    , flags(std::move(flags))
{}

// template<typename T>
bool ParserParameter::getIsFlagPresent(std::string flag) {
    for (int i = 0; i < this->flags.size(); i++) {
        if (this->flags[i] == flag) {
            return true;
        }
    }

    return false;
}

// template<typename T>
std::pair<bool, std::string> ParserParameter::validate(std::string input) {
    bool isValid = std::regex_match(input, this->validator);
    return std::make_pair(isValid, input);
}
