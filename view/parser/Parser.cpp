#include "Parser.h"

#include <iostream>

#include "view/ViewHelper.h"

// template<typename T>
Parser::Parser(std::map<std::string, ParserCommandInfo> scheme)
    : scheme(std::move(scheme))
{}

// template<typename T>
std::vector<std::string> Parser::split(const std::string &initial, char delim) {
    std::vector<std::string> elems;
    std::string current;

    for (int i = 0; i < initial.length(); i++) {
        if (initial[i] == delim) {
            elems.push_back(current);
            current = "";
        } else {
            current += initial[i];
        }
    }

    if (!current.empty()) {
        elems.push_back(current);
    }
    return elems;
}

// template<typename T>
ParserParameter* Parser::findOption(const std::string &flag, ParserCommandInfo& command) {
    for (int i = 0; i < command.getParams().size(); i++) {
        ParserParameter param = command.getParams()[i];
        if (param.getIsFlagPresent(flag)) return &command.getParams()[i];
    }

    return nullptr;
}

// template<typename T>
std::pair<bool, std::map<std::string, std::string>> Parser::validateParams(std::vector<std::string> &inputChunks, ParserCommandInfo& command) {
    bool isValid = true;
    std::map<std::string, std::string> validParamValues;

    for (int i = 0; i < inputChunks.size(); i++) {
        std::string chunk = inputChunks[i];
        if (chunk.substr(0, 2) == "--") {
            std::cout << chunk << std::endl;
            ParserParameter* option = findOption(chunk, command);
            std::string optionValue = i == inputChunks.size() - 1 ? "" : inputChunks[i + 1];
            std::pair<bool, std::string> validationResult = option->validate(optionValue);
            isValid = isValid && validationResult.first;
            if (isValid) validParamValues.emplace(chunk.substr(2), validationResult.second);
        }
    }

    return std::make_pair(isValid, validParamValues);
}

// template<typename T>
void displayError(std::map<std::string, std::string> args) {
    ViewHelper::consoleOut("Error: one or more arguments is invalid. Please try again.");
}

// template<typename T>
std::pair<std::function<void(std::map<std::string, std::string>)>, std::map<std::string, std::string>> Parser::parse(std::string input) {
    std::vector<std::string> splitInput = this->split(input, ' ');
    if (splitInput.empty()) throw std::invalid_argument("Invalid input");

    ParserCommandInfo relatedCommand = this->scheme.at(splitInput[0]);
    std::pair<bool, std::map<std::string, std::string>> validationResult = this->validateParams(splitInput, relatedCommand);
    std::map<std::string, std::string> parsedArguments = validationResult.second;

    if (!validationResult.first) {
        return std::make_pair(displayError, parsedArguments);
    }

    return std::make_pair(relatedCommand.getExecutable(parsedArguments), parsedArguments);
}
