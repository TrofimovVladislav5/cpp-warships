#pragma once
#include <input_parser/model/ParserCommandInfo.h>

namespace cpp_warships::input_parser {
    class DefaultParserError {
    public:
        static void CommandNotFoundError(model::ParsedOptions options);
        static void WrongFlagValueError(model::ParsedOptions options);
    };
}  // namespace cpp_warships::input_parser