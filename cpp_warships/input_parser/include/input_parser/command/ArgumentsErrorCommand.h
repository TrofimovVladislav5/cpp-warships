#pragma once
#include <input_parser/command/ParserCommand.h>

#include <utility>

namespace cpp_warships::input_parser {
    class ArgumentsErrorCommand : public ParserCommand {
       private:
        ParserCommandInfo<ParserCommand*> command;

       public:
        explicit ArgumentsErrorCommand(ParserCommandInfo<ParserCommand*> command)
            : ParserCommand(), command(std::move(command)) {
        }

        void execute(ParsedOptions options) override;
    };
}  // namespace cpp_warships::input_parser