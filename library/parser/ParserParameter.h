#pragma once
#include <regex>
#include <vector>
#include <string>

class ParserParameter {
private:
    std::regex validator;
    std::string description;
    std::vector<std::string> flags;
    bool necessary = false;
public:
    ParserParameter() = default;
    ParserParameter(std::vector<std::string> flags, std::regex validator, std::string description, bool necessary);

    std::string getDescription() const;
    bool getNecessary() const;
    bool getIsFlagPresent(const std::string &flag) const;
    std::vector<std::string> getFlags() const;
    std::pair<bool, std::string> validate(const std::string& input) const;
};