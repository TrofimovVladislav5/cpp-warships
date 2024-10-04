#include "ParserParameter.h"


// template<typename T>
ParserParameter::ParserParameter() = default;

// template<typename T>
ParserParameter::ParserParameter(std::vector<std::string> flags, std::regex validator)
    : validator(std::move(validator))
    , flags(std::move(flags))
{}

// template<typename T>
ParserParameter::ParserParameter(std::vector<std::string> flags, std::regex validator, std::string description)
    : validator(std::move(validator))
    , description(std::move(description))
    , flags(std::move(flags))
{}

// template<typename T>
std::string ParserParameter::getDescription() {
    return this->description;
}

// template<typename T>
bool ParserParameter::getIsFlagPresent(const std::string &flag) const {
    for (int i = 0; i < this->flags.size(); i++) {
        if (this->flags[i] == flag) {
            return true;
        }
    }

    return false;
}

// template<typename T>
std::pair<bool, std::string> ParserParameter::validate(const std::string& input) const {
    bool isValid = std::regex_match(input, this->validator);

    return std::make_pair(isValid, input);
}
