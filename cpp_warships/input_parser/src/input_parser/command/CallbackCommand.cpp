#include <input_parser/command/CallbackCommand.h>

#include <functional>
#include <utility>

namespace cpp_warships::input_parser::command {
    CallbackCommand::CallbackCommand(std::function<void(model::ParsedOptions options)> callback)
        : ParserCommand()
        , callback(std::move(callback)) {}

    void CallbackCommand::execute(model::ParsedOptions options) {
        callback(options);
    }
}  // namespace cpp_warships::input_parser::command
