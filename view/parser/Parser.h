#pragma once
#include <map>
#include <regex>

#include "ParserCommandInfo.h"

// typedef std::pair<bool, std::map<std::string, std::string>> ParseResult;

// template <typename T>
// using SchemeMap = std::map<std::string, ParserCommandInfo<T>>;

//TODO: dynamic typing for scheme commands, dynamic typing for parse

// template<typename T>
class Parser {
private:
    std::map<std::string, ParserCommandInfo> scheme;
    std::vector<std::string> split(const std::string &s, char delim);
    ParserParameter* findOption(const std::string &flag, ParserCommandInfo &command);
    std::pair<bool, std::map<std::string, std::string>> validateParams(std::vector<std::string> &inputChunks, ParserCommandInfo &command);
public:
    Parser(std::map<std::string, ParserCommandInfo> scheme);
    std::pair<std::function<void(std::map<std::string, std::string>)>, std::map<std::string, std::string>> parse(std::string input);
};