#include "Parser.h"

#include "library/StringHelper.h"
#include "view/ViewHelper.h"

// template<typename T>
bool findOption(const std::string &flag, const ParserCommandInfo& command, ParserParameter& result);

// template<typename T>
void displayError(ParsedOptions args);

// template<typename T>
bool necessaryFlagsPresent(const std::vector<std::string> &input, const ParserCommandInfo &scheme);

// template<typename T>
bool commandInScheme(std::string &command, const SchemeMap &scheme);


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
std::pair<ParseCallback, ParsedOptions> Parser::parse(const std::string &input) {
    std::vector<std::string> splitInput = StringHelper::split(input, ' ');
    if (
        splitInput.empty() ||
        !commandInScheme(splitInput[0], this->scheme) ||
        !necessaryFlagsPresent(splitInput, this->scheme.at(splitInput[0]))
    ) {
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

// template<typename T>
void displayError(ParsedOptions args) {
    ViewHelper::consoleOut("Error: one or more arguments is invalid. Please try again.");
    auto begin = args.begin();
    for (int i = 0; i < args.size(); i++) {
        ViewHelper::consoleOut(begin->first + ": " + begin->second);
        std::advance(begin, 1);
    }
}

bool commandInScheme(std::string &command, const SchemeMap &scheme) {
    return scheme.find(command) != scheme.end();
}

void increaseAmountOnHashMap(std::string &key, std::unordered_map<std::string, int> &map) {
    if (map.find(key) == map.end()) {
        map[key] = 1;
    } else {
        map[key]++;
    }
}

void decreaseAmountOnHashMap(std::string &key, std::unordered_map<std::string, int> &map) {
    if (map.find(key) != map.end()) {
        map[key]--;
    }
}

bool necessaryFlagsPresent(const std::vector<std::string> &input, const ParserCommandInfo &scheme) {
    auto params = scheme.getParams();

    std::unordered_map<std::string, int> necessaryFlags;
    std::unordered_map<std::string, int> allFlags;

    for (const auto& param : params) {
        std::vector<std::string> flags = param.getFlags();
        bool isNecessary = param.getNecessary();

        for (int j = 0; j < flags.size(); j++) {
            increaseAmountOnHashMap(flags[j], allFlags);
            if (isNecessary) increaseAmountOnHashMap(flags[j], necessaryFlags);
        }
    }

    for (auto currentChunk : input) {
        decreaseAmountOnHashMap(currentChunk, necessaryFlags);
        decreaseAmountOnHashMap(currentChunk, allFlags);
    }

    for (auto & necessaryFlag : necessaryFlags) {
        if (necessaryFlag.second > 0) return false;
    }

    for (auto & allFlag : allFlags) {
        if (allFlag.second < 0) return false;
    }

    return true;
}