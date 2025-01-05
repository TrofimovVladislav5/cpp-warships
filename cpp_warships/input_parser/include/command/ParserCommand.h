#pragma once
#include "Command.h"
#include "../model/ParserCommandInfo.h"

namespace cpp_warships::input_parser {
    class ParserCommand : public Command<ParsedOptions> {};
}
