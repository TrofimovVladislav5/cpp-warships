#include "ParserParameter.h"

template<typename T>
ParserParameter<T>::ParserParameter(std::vector<std::string> flags, std::regex validator)
    : validator(std::move(validator))
    , flags(std::move(flags))
{}

template<typename T>
std::string ParserParameter<T>::getDescription() {
    return this->description;
}

template<typename T>
ParserParameter<T>::ParserParameter(std::vector<std::string> flags, std::regex validator, std::string description)
    : validator(std::move(validator))
    , description(std::move(description))
    , flags(std::move(flags))
{}

template<typename T>
bool ParserParameter<T>::getIsFlagPresent(std::string flag) {
    for (int i = 0; i < this->flags.size(); i++) {
        if (this->flags[i] == flag) {
            return true;
        }
    }

    return false;
}

template<typename T>
std::pair<bool, std::string> ParserParameter<T>::validate(std::string input) {
    bool isValid = std::regex_match(input, this->validator);
    return std::make_pair(isValid, isValid ? static_cast<T>(input) : static_cast<T>(0));
}
