#include "Parser.h"

#include "library/StringHelper.h"
#include "../ViewHelper.h"
#include "library/TypesHelper.h"
#include "library/defaults/DefaultHelp.h"
#include "library/parser-builder/ConfigCommandBuilder.h"

// template<typename T>
bool findOption(const std::string &flag, const ParserCommandInfo& command, ParserParameter& result);

// template<typename T>
bool necessaryFlagsPresent(const std::vector<std::string> &input, const ParserCommandInfo &scheme);

// template<typename T>
bool commandInScheme(std::string &command, const SchemeMap &scheme);

// template<typename T>
Parser::Parser(SchemeMap scheme)
    : scheme(std::move(scheme))
{}

// template<typename T>
void Parser::printCommandsHelp(ParsedOptions options) {
    ViewHelper::consoleOut("This is the list of supported commands:");

    for (const auto& command : scheme) {
        auto commandPrint = command.second.getPrintHelp();
        if (commandPrint) {
            commandPrint(options);
        } else {
            DefaultHelp::PrintCommand(command, DefaultHelp::PrintParam);
        }
    }
}

// template<typename T>
Parser::Parser(SchemeMap scheme, ParseCallback displayError, const SchemeHelpCallback& printHelp)
    : scheme(std::move(scheme))
    , displayError(std::move(displayError))
{
    if (scheme.find("help") == scheme.end()) {
        ConfigCommandBuilder commandBuilder;
        ParserCommandInfo* helpInfo;

        if (printHelp) {
            helpInfo = new ParserCommandInfo({
                commandBuilder
                    .setDescription("Command to display this message")
                    .setCallback(std::bind(printHelp, scheme))
                    .buildAndReset()
            });
        } else {
            helpInfo = new ParserCommandInfo({
                commandBuilder
                    .setDescription("Command to display this message")
                    .setCallback(TypesHelper::methodToFunction(&Parser::printCommandsHelp, this))
                    .buildAndReset()
            });
        }

        this->scheme.insert({"help", *helpInfo});
        delete helpInfo;
    }
}

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
            } else if (command.getResolveAllFlags()) {
                isValid = false;
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
        ParseCallback commandNotFound = this->displayError
            ? this->displayError
            : throw std::invalid_argument("Command not found. You can get better error message by providing displayError callback");
        return std::make_pair(commandNotFound, ParsedOptions());
    }

    ParserCommandInfo relatedCommand = this->scheme.at(splitInput[0]);
    const std::pair<bool, ParsedOptions> validationResult = this->validateParams(splitInput, relatedCommand);
    ParsedOptions parsedArguments = validationResult.second;

    if (!validationResult.first) {
        ParseCallback protectedDisplayError = relatedCommand.getErrorDisplay()
            ? relatedCommand.getErrorDisplay()
            : throw std::invalid_argument("Arguments validation failed. You can get better error message by providing displayError callback");
        return std::make_pair(protectedDisplayError, parsedArguments);
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