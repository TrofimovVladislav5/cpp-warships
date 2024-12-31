#include "ArgumentsErrorCommand.h"

void ArgumentsErrorCommand::execute(ParsedOptions options) {
    ParseCallback<void> protectedDisplayError = command.getErrorDisplay()
        ? command.getErrorDisplay()
        : throw std::invalid_argument("Arguments validation failed. You can get better error message by providing displayError callback");

    protectedDisplayError(options);
}
