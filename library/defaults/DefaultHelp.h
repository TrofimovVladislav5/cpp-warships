#pragma once
#include "library/parser/ParserCommandInfo.h"
#include "library/parser/ParserParameter.h"

class DefaultHelp {
public:
    static void PrintParam(const ParserParameter &param);
    static void PrintCommand(const std::pair<std::string, ParserCommandInfo>& command, const std::function<void(ParserParameter)> &printParam);
};
