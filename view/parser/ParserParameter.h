#pragma once
#include <regex>

// template<typename T>
class ParserParameter {
private:
    std::regex validator;
    std::string description;
    std::vector<std::string> flags;
public:
    ParserParameter(std::vector<std::string> flags, std::regex validator);
    ParserParameter(std::vector<std::string> flags, std::regex validator, std::string description);
    std::string getDescription();
    bool getIsFlagPresent(std::string flag);
    std::pair<bool, std::string> validate(std::string input);
};
