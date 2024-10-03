#include "Parser.h"

#include "view/ViewHelper.h"

template<typename T>
Parser<T>::Parser(std::map<std::string, ParserCommandInfo<T>> scheme)
    : scheme(std::move(scheme))
{}

template<typename T>
std::vector<std::string> Parser<T>::split(const std::string &initial, char delim) {
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
}

template<typename T>
ParserParameter<std::string>* Parser<T>::findOption(const std::string &flag, ParserCommandInfo<void>& command) {
    for (int i = 0; i < command.getParams().size(); i++) {
        ParserParameter<std::string> param = command.getParams()[i];
        if (param.getIsFlagPresent(flag)) return &command.getParams()[i];
    }

    return nullptr;
}

template<typename T>
ParseResult Parser<T>::validateParams(std::vector<std::string> &inputChunks, ParserCommandInfo<void>& command) {
    bool isValid = true;
    std::map<std::string, std::string> validParamValues;

    for (int i = 0; i < inputChunks.size(); i++) {
        std::string chunk = inputChunks[i];
        if (chunk.substr(0, 2) == "--") {
            ParserParameter<std::string> option = findOption(chunk, command);
            std::string optionValue = i == inputChunks.size() ? "" : inputChunks[i + 1];
            std::pair<bool, std::string> validationResult = option.validate(optionValue);
            isValid = isValid && validationResult.first;
            if (isValid) validParamValues.insert(chunk.substr(2), validationResult.second);
        }
    }

    return std::make_pair(isValid, validParamValues);
}

template<typename T>
void Parser<T>::displayError() {
    ViewHelper::consoleOut("Error: one or more arguments is invalid. Please try again.");
}

template<typename T>
std::function<void(std::map<std::string, std::string>)> Parser<T>::parse(std::string input) {
    std::vector<std::string> splitInput = this->split(input, ' ');
    if (splitInput.empty()) throw std::invalid_argument("Invalid input");

    ParserCommandInfo<void> relatedCommand = this->scheme[splitInput[0]];
    ParseResult validationResult = this->validateParams(splitInput, relatedCommand);
    if (!validationResult.first) {
        return this->displayError;
    }

    std::map<std::string, std::string> parsedArguments = validationResult.second;
    return relatedCommand.getExecutable(parsedArguments);
}
