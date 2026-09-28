#pragma once
#include <input_parser/model/ParserCommandInfo.h>

#include <map>

namespace cpp_warships::input_parser::command {
    template <typename T>
    class Command {
    public:
        virtual ~Command() = default;

        virtual void execute(T data) = 0;
    };
}  // namespace cpp_warships::input_parser::command