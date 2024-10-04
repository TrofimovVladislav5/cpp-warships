#pragma once
#include <map>
#include <regex>

#include "ParserCommandInfo.h"

// template <typename T>
typedef std::map<std::string, ParserCommandInfo> SchemeMap;

//TODO: dynamic typing for scheme commands, dynamic typing for parse

// template<typename T>
class Parser {
private:
    SchemeMap scheme;

    static std::vector<std::string> split(const std::string &initial, char delim);
    static bool findOption(const std::string &flag, const ParserCommandInfo &command, ParserParameter &result);
    static std::pair<bool, ParsedOptions> validateParams(const std::vector<std::string> &inputChunks, ParserCommandInfo &command);
public:
    explicit Parser(SchemeMap scheme);
    std::pair<ParseCallback, ParsedOptions> parse(const std::string &input) const;
};