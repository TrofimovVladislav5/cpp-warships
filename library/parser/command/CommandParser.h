#pragma once
#include "Command.h"
#include "Parser.h"

using ParseResult = std::pair<ParseCallback<ParserCommand*>, ParsedOptions>;


class CommandParser : Parser<ParserCommand*> {
public:
    explicit CommandParser(const SchemeMap<ParserCommand*> &scheme)
        : Parser<ParserCommand*>(scheme) {
    }

    CommandParser(
        const SchemeMap<ParserCommand*> &scheme,
        const ParseCallback<void> &displayError,
        const SchemeHelpCallback<void> &printHelp = nullptr
    )
        : Parser<ParserCommand*>(scheme, displayError, printHelp)
    {}

    ~CommandParser() override = default;

    void executedParse(const std::string &input) override {
        ParseResult parseResult = this->parse(input);
        if (parseResult.first) {
            ParserCommand* command = parseResult.first(parseResult.second);
            command->execute(parseResult.second);
        }
    }

    BindedParseCallback<ParserCommand*> bindedParse(const std::string &input) override {
        ParseResult result = this->parse(input);
        return std::bind(result.first, result.second);
    }
};
