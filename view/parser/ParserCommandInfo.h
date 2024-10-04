#pragma once
#include <map>
#include <string>

#include "ParserParameter.h"

// template <typename T>
typedef std::map<std::string, std::string> ParsedOptions;

// template <typename T>
typedef std::function<void(ParsedOptions)> ParseCallback;

// template<typename T>
struct ParserCommandInfoConfig {
    ParserCommandInfoConfig(
        std::string description,
        std::vector<ParserParameter> parameters,
        ParseCallback function
    );

    std::string description;
    std::vector<ParserParameter> parameters;
    ParseCallback executable;
};

// template<typename T>
class ParserCommandInfo {
private:
    ParserCommandInfoConfig config;
public:
    explicit ParserCommandInfo(ParserCommandInfoConfig config);
    std::vector<ParserParameter> getParams() const;
    std::string getDescription() const;
    ParseCallback getExecutable() const;
};