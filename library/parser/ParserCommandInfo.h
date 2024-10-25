#pragma once
#include <iostream>
#include <map>
#include <string>

#include "ParserParameter.h"


// template <typename T>
typedef std::map<std::string, std::string> ParsedOptions;

// template <typename T>
typedef std::function<void(ParsedOptions)> ParseCallback;

//template <typename T>
typedef std::function<void()> BindedParseCallback;

struct ParserCommandInfoConfig {
    ParserCommandInfoConfig(
        std::string description,
        std::vector<ParserParameter> parameters,
        ParseCallback function,
        ParseCallback displayError,
        bool requireAllFlags,
        ParseCallback printHelp
    );

    ParserCommandInfoConfig(
        std::string description,
        std::vector<ParserParameter> parameters,
        ParseCallback function
    );

    bool resolveAllFlags = false;
    std::string description;
    std::vector<ParserParameter> parameters;
    ParseCallback executable;
    ParseCallback displayError;
    ParseCallback printHelp;
};

// template<typename T>
class ParserCommandInfo {
private:
    ParserCommandInfoConfig config;
public:
    explicit ParserCommandInfo(ParserCommandInfoConfig config);
    std::vector<ParserParameter> getParams() const;
    std::string getDescription() const;
    bool getResolveAllFlags() const;
    ParseCallback getErrorDisplay() const;
    ParseCallback getExecutable() const;
    ParseCallback getPrintHelp() const;
};