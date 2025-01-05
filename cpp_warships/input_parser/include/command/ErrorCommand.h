#pragma once
#include "ParserCommand.h"
#include "../model/Parser.h"


namespace cpp_warships::input_parser {
    class ErrorCommand : public ParserCommand {
    private:
        std::function<void(ParsedOptions options)> displayError;
    public:
        explicit ErrorCommand(std::function<void(ParsedOptions options)> displayError);

        void execute(ParsedOptions options) override;
    };
}