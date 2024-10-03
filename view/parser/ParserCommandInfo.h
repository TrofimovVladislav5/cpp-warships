#pragma once
#include <map>
#include <string>

#include "ParserParameter.h"

//FIXME: Remove
// start --size 10 --time-limit 300 --verbose
// insert(4 qwe)


//TODO: add aliases (?)
//TODO: Replace 'string' with list of types in template

template<typename T> struct ParserCommandInfoConfig {
    ParserCommandInfoConfig(
        std::string description,
        std::vector<ParserParameter<std::string>> parameters,
        std::function<void(std::map<std::string, std::string>)> function
    );

    std::string description;
    std::vector<ParserParameter<std::string>> parameters;
    std::function<void(std::map<std::string, std::string>)> executable;
};

template<typename T> class ParserCommandInfo {
private:
    ParserCommandInfoConfig<T> config;
public:
    ParserCommandInfo(ParserCommandInfoConfig<std::string> config);
    std::vector<ParserParameter<std::string>> getParams();
    std::string getDescription();
    std::function<void(std::map<std::string, std::string>)> getExecutable(std::map<std::string, std::string>);
};