#pragma once
#include <map>
#include <regex>

#include "ParserCommandInfo.h"

typedef std::pair<bool, std::map<std::string, std::string>> ParseResult;

// template <typename T>
// using SchemeMap = std::map<std::string, ParserCommandInfo<T>>;

//TODO: dynamic typing for scheme commands, dynamic typing for parse
template<typename T> class Parser {
private:
    std::map<std::string, ParserCommandInfo<void>> scheme;
    std::vector<std::string> split(const std::string &s, char delim);
    void displayError();
    ParserParameter<std::string>* findOption(const std::string &flag, ParserCommandInfo<void> &command);
    ParseResult validateParams(std::vector<std::string> &inputChunks, ParserCommandInfo<void> &command);
public:
    Parser(std::map<std::string, ParserCommandInfo<T>> scheme);
    std::function<void(std::map<std::string, std::string>)> parse(std::string input);
};