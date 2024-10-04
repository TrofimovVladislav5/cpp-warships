#pragma once
#include <regex>
#include <vector>
#include <string>

class ParserParameter {
private:
    std::regex validator;
    std::string description;
    std::vector<std::string> flags;
public:
    ParserParameter();
    ParserParameter(std::vector<std::string> flags, std::regex validator);
    ParserParameter(std::vector<std::string> flags, std::regex validator, std::string description);
    std::string getDescription();
    bool getIsFlagPresent(const std::string &flag) const;
    std::pair<bool, std::string> validate(const std::string& input) const;
};