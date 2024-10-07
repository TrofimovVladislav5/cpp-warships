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
    ParseCallback displayError;
    std::pair<bool, ParsedOptions> validateParams(const std::vector<std::string> &inputChunks, ParserCommandInfo &command);
public:
    explicit Parser(SchemeMap scheme);
    explicit Parser(SchemeMap scheme, ParseCallback displayError);

    std::pair<ParseCallback, ParsedOptions> parse(const std::string &input);
    BindedParseCallback bindedParse(const std::string &input);
    void executedParse(const std::string &input);
};