#pragma once
#include <input_parser/command/Command.h>
#include <input_parser/model/ParserCommandInfo.h>

namespace cpp_warships::input_parser::command {
    class ParserCommand : public Command<model::ParsedOptions> {};
}  // namespace cpp_warships::input_parser::command
