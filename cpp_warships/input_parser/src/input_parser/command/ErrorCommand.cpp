#include <input_parser/command/ErrorCommand.h>

namespace cpp_warships::input_parser::command {
    ErrorCommand::ErrorCommand(std::function<void(model::ParsedOptions options)> displayError)
        : ParserCommand()
        , displayError(std::move(displayError)) {}

    void ErrorCommand::execute(model::ParsedOptions options) {
        displayError(options);
    }
}  // namespace cpp_warships::input_parser::command