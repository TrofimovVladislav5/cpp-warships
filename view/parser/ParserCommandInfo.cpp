#include "ParserCommandInfo.h"

#include <map>

template<typename T>
ParserCommandInfoConfig<T>::ParserCommandInfoConfig(
    std::string description,
    std::vector<ParserParameter<std::string> > parameters,
    std::function<void(std::map<std::string, std::string>)> function
)
    : description(std::move(description))
    , parameters(std::move(parameters))
    , executable(std::move(function))
{}


template<typename T>
ParserCommandInfo<T>::ParserCommandInfo(ParserCommandInfoConfig<std::string> config)
    : config(config)
{}

template<typename T>
std::string ParserCommandInfo<T>::getDescription() {
    return this->config.getDescription();
}

template<typename T>
std::function<void(std::map<std::string, std::string>)> ParserCommandInfo<T>::getExecutable(std::map<std::string, std::string>) {
    return this->config.getExecutable();
}

template<typename T>
std::vector<ParserParameter<std::string> > ParserCommandInfo<T>::getParams() {
    return this->config.getParams();
}
