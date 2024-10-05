#include "Parser.h"

#include "library/StringHelper.h"
#include "view/ViewHelper.h"

// template<typename T>
bool findOption(const std::string &flag, const ParserCommandInfo& command, ParserParameter& result);

// template<typename T>
Parser::Parser(SchemeMap scheme)
    : scheme(std::move(scheme))
{}

// template<typename T>
std::pair<bool, ParsedOptions> Parser::validateParams(const std::vector<std::string> &inputChunks, ParserCommandInfo& command) {
    bool isValid = true;
    ParsedOptions validParamValues;

    for (int i = 0; i < inputChunks.size(); i++) {
        const std::string& chunk = inputChunks[i];
        if (chunk.substr(0, 2) == "--") {
            ParserParameter option;
            if (findOption(chunk, command, option)) {
                std::string optionValue = i == inputChunks.size() - 1 ? "" : inputChunks[i + 1];
                std::pair<bool, std::string> validationResult = option.validate(optionValue);
                isValid = isValid && validationResult.first;
                if (isValid) validParamValues.emplace(chunk.substr(2), validationResult.second);
            }
        }
    }

    return std::make_pair(isValid, validParamValues);
}

// template<typename T>
void displayError(ParsedOptions args) {
    ViewHelper::consoleOut("Error: one or more arguments is invalid. Please try again.");
    auto begin = args.begin();
    for (int i = 0; i < args.size(); i++) {
        ViewHelper::consoleOut(begin->first + ": " + begin->second);
        std::advance(begin, 1);
    }
}

// template<typename T>
std::pair<ParseCallback, ParsedOptions> Parser::parse(const std::string &input) {
    std::vector<std::string> splitInput = StringHelper::split(input, ' ');
    if (splitInput.empty() || this->scheme.find(splitInput[0]) == this->scheme.end()) {
        throw std::invalid_argument("Invalid input");
    }

    ParserCommandInfo relatedCommand = this->scheme.at(splitInput[0]);
    const std::pair<bool, ParsedOptions> validationResult = this->validateParams(splitInput, relatedCommand);
    ParsedOptions parsedArguments = validationResult.second;

    if (!validationResult.first) {
        return std::make_pair(displayError, parsedArguments);
    }

    return std::make_pair(relatedCommand.getExecutable(), parsedArguments);
}

//template<typename T>
BindedParseCallback Parser::bindedParse(const std::string &input) {
    std::pair<ParseCallback, ParsedOptions> result = this->parse(input);
    return std::bind(result.first, result.second);
}

void Parser::executedParse(const std::string &input) {
    std::pair<ParseCallback, ParsedOptions> result = this->parse(input);
    result.first(result.second);
}

// template<typename T>
bool findOption(const std::string &flag, const ParserCommandInfo& command, ParserParameter& result) {
    for (int i = 0; i < command.getParams().size(); i++) {
        ParserParameter param = command.getParams()[i];
        if (param.getIsFlagPresent(flag)) {
            result = param;
            return true;
        }
    }

    return false;
}