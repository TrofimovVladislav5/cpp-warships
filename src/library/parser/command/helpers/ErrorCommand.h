#pragma once
#include "Parser.h"
#include "ParserCommand.h"


class ErrorCommand : public ParserCommand {
private:
    std::function<void(ParsedOptions options)> displayError;
public:
    explicit ErrorCommand(std::function<void(ParsedOptions options)> displayError);

    void execute(ParsedOptions options) override;
};
