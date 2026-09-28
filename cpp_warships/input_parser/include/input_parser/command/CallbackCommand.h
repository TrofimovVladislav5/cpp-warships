#pragma once
#include <input_parser/command/ParserCommand.h>
#include <input_parser/model/Parser.h>

#include <functional>

namespace cpp_warships::input_parser::command {
    /** @brief A command that is nothing but the callback it was handed, for when a caller
     * supplies its own rendering rather than one of the parser's own commands. */
    class CallbackCommand : public ParserCommand {
    private:
        std::function<void(model::ParsedOptions options)> callback;

    public:
        explicit CallbackCommand(std::function<void(model::ParsedOptions options)> callback);

        void execute(model::ParsedOptions options) override;
    };
}  // namespace cpp_warships::input_parser::command
