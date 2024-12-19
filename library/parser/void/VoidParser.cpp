#include "VoidParser.h"

#include "ParserCommandInfo.h"


VoidParser::VoidParser(const SchemeMap<void> &scheme)
    : Parser<void>(scheme)
{}

VoidParser::VoidParser(
    const SchemeMap<void> &scheme, const ParseCallback<void> &displayError,
    const SchemeHelpCallback<void> &printHelp
)
    : Parser<>(scheme, displayError, printHelp)
{}

BindedParseCallback<void> VoidParser::bindedParse(const std::string &input) {
    std::pair<ParseCallback<void>, ParsedOptions> result = this->parse(input);
    return std::bind(result.first, result.second);
}

void VoidParser::executedParse(const std::string &input) {
    std::pair<ParseCallback<void>, ParsedOptions> result = this->parse(input);
    result.first(result.second);
}