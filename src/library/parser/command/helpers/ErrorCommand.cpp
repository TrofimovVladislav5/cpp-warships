#include "ErrorCommand.h"

ErrorCommand::ErrorCommand(std::function<void(ParsedOptions options)> displayError)
    : ParserCommand()
    , displayError(std::move(displayError))
{}

void ErrorCommand::execute(ParsedOptions options) {
    displayError(options);
}