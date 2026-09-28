#pragma once
#include <input_parser/command/ParserCommand.h>

#include <utility>

namespace cpp_warships::input_parser::command {
    class ArgumentsErrorCommand : public ParserCommand {
    private:
        model::ParserCommandInfo<ParserCommand*> command;

    public:
        explicit ArgumentsErrorCommand(model::ParserCommandInfo<ParserCommand*> command)
            : ParserCommand()
            , command(std::move(command)) {}

        void execute(model::ParsedOptions options) override;
    };
}  // namespace cpp_warships::input_parser::command