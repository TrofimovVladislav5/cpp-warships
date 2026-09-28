#pragma once
#include <input_parser/command/ParserCommand.h>
#include <input_parser/model/Parser.h>

namespace cpp_warships::input_parser::command {
    class ErrorCommand : public ParserCommand {
    private:
        std::function<void(model::ParsedOptions options)> displayError;

    public:
        explicit ErrorCommand(std::function<void(model::ParsedOptions options)> displayError);

        void execute(model::ParsedOptions options) override;
    };
}  // namespace cpp_warships::input_parser::command