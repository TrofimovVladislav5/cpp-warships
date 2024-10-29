#include "ParserParameter.h"
#include "ParserParameter.h"


// template<typename T>
ParserParameter::ParserParameter(
    std::vector<std::string> flags,
    std::regex validator,
    std::string description,
    const bool necessary
)
    : validator(std::move(validator))
    , description(std::move(description))
    , flags(std::move(flags))
    , necessary(necessary)
{}

// template<typename T>
std::string ParserParameter::getDescription() const {
    return this->description;
}

// template<typename T>
bool ParserParameter::getNecessary() const {
    return this->necessary;
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
std::vector<std::string> ParserParameter::getFlags() const {
    return this->flags;
}

// template<typename T>
std::pair<bool, std::string> ParserParameter::validate(const std::string& input) const {
    bool isValid = std::regex_match(input, this->validator);

    return std::make_pair(isValid, input);
}
