#include <input_parser/command/ArgumentsErrorCommand.h>

namespace cpp_warships::input_parser::command {
    void ArgumentsErrorCommand::execute(model::ParsedOptions options) {
        model::ParseCallback<void> protectedDisplayError =
            command.getErrorDisplay() ? command.getErrorDisplay()
                                      : throw std::invalid_argument(
                                            "Arguments validation failed. You can "
                                            "get better error message "
                                            "by providing displayError callback"
                                        );

        protectedDisplayError(options);
    }
}  // namespace cpp_warships::input_parser::command