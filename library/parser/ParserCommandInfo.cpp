#include "ParserCommandInfo.h"

#include "Parser.h"
#include "library/TypesHelper.h"

// template<typename T>
ParserCommandInfoConfig::ParserCommandInfoConfig(
    std::string description,
    std::vector<ParserParameter> parameters,
    ParseCallback function,
    ParseCallback displayError,
    bool resolveAllFlags,
    ParseCallback printHelp
)
    : description(std::move(description))
    , parameters(std::move(parameters))
    , executable(std::move(function))
    , displayError(std::move(displayError))
    , resolveAllFlags(resolveAllFlags)
    , printHelp(std::move(printHelp))
{}

// template<typename T>
ParserCommandInfoConfig::ParserCommandInfoConfig(
    std::string description,
    std::vector<ParserParameter> parameters,
    ParseCallback function
)
    : description(std::move(description))
    , parameters(std::move(parameters))
    , executable(std::move(function))
{}

// template<typename T>
ParserCommandInfo::ParserCommandInfo(ParserCommandInfoConfig config)
    : config(std::move(config))
{}

// template<typename T>
std::string ParserCommandInfo::getDescription() const {
    return this->config.description;
}

// template<typename T>
bool ParserCommandInfo::getResolveAllFlags() const {
    return this->config.resolveAllFlags;
}

// template<typename T>
ParseCallback ParserCommandInfo::getErrorDisplay() const {
    return this->config.displayError;
}

// template<typename T>
ParseCallback ParserCommandInfo::getExecutable() const {
    return this->config.executable;
}

// template<typename T>
std::vector<ParserParameter> ParserCommandInfo::getParams() const {
    return this->config.parameters;
}

// template<typename T>
ParseCallback ParserCommandInfo::getPrintHelp() const {
    return this->config.printHelp;
}