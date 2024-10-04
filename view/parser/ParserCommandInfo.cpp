#include "ParserCommandInfo.h"

#include <map>

// template<typename T>
ParserCommandInfoConfig::ParserCommandInfoConfig(
    std::string description,
    std::vector<ParserParameter> parameters,
    std::function<void(std::map<std::string, std::string>)> function
)
    : description(std::move(description))
    , parameters(std::move(parameters))
    , executable(std::move(function))
{}


// template<typename T>
ParserCommandInfo::ParserCommandInfo(ParserCommandInfoConfig config)
    : config(config)
{}

// template<typename T>
std::string ParserCommandInfo::getDescription() {
    return this->config.description;
}

// template<typename T>
std::function<void(std::map<std::string, std::string>)> ParserCommandInfo::getExecutable(std::map<std::string, std::string>) {
    return this->config.executable;
}

// template<typename T>
std::vector<ParserParameter> ParserCommandInfo::getParams() {
    return this->config.parameters;
}
