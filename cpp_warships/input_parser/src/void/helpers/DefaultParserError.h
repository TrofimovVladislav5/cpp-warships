#pragma once
#include "../../ParserCommandInfo.h"

class DefaultParserError {
public:
    static void CommandNotFoundError(ParsedOptions options);
    static void WrongFlagValueError(ParsedOptions options);
};
