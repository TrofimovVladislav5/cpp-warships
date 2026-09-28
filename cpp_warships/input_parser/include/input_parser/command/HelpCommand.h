#pragma once
#include <input_parser/command/ParserCommand.h>
#include <input_parser/model/Parser.h>

namespace cpp_warships::input_parser::command {
    class HelpCommand : public ParserCommand {
    private:
        model::SchemeMap<ParserCommand*> scheme;

    public:
        explicit HelpCommand(model::SchemeMap<ParserCommand*> scheme);

        void execute(model::ParsedOptions options) override;
    };
}  // namespace cpp_warships::input_parser::command