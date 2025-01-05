#include "CommandParser.h"

#include "../builder/ConfigCommandBuilder.h"
#include "helpers/ArgumentsErrorCommand.h"
#include "helpers/ErrorCommand.h"
#include "helpers/HelpCommand.h"


ParserCommand* CommandParser::printCommandsHelp(ParsedOptions options) {
    return new HelpCommand(this->scheme);
}

ParserCommand* CommandParser::printCommandsError(ParsedOptions options) {
    return new ErrorCommand(this->displayError);
}

ParserCommand* CommandParser::printArgumentsError(
    ParserCommandInfo<ParserCommand *> command,
    ParsedOptions options
) {
    return new ArgumentsErrorCommand(command);
}

CommandParser::CommandParser(const SchemeMap<ParserCommand*> &scheme)
        : Parser<ParserCommand*>(scheme)
{}

CommandParser::CommandParser(
    const SchemeMap<ParserCommand*> &scheme,
    const ParseCallback<void> &displayError,
    const SchemeHelpCallback<void> &printHelp
)
    : Parser<ParserCommand*>(scheme, displayError)
{
    if (scheme.find("help") == scheme.end()) {
        ConfigCommandBuilder<ParserCommand*> commandBuilder;
        ParserCommandInfo<ParserCommand*>* helpInfo;

        if (printHelp) {
            helpInfo = new ParserCommandInfo<ParserCommand*>({
                commandBuilder
                    .setDescription("Command to display this message")
                    .setCallback(TypesHelper::methodToFunction(&CommandParser::printCommandsHelp, this))
                    .buildAndReset()
            });
        } else {
            auto method = TypesHelper::methodToFunction(&CommandParser::printCommandsHelp, this);
            helpInfo = new ParserCommandInfo<ParserCommand*>({
                commandBuilder
                    .setDescription("Command to display this message")
                    .setCallback(TypesHelper::methodToFunction(&CommandParser::printCommandsHelp, this))
                    .buildAndReset()
            });
        }

        this->scheme.insert({"help", *helpInfo});
        delete helpInfo;
    }
}

void CommandParser::executedParse(const std::string &input) {
    ParseResult parseResult = this->parse(input);
    if (parseResult.first) {
        ParserCommand* command = parseResult.first(parseResult.second);
        command->execute(parseResult.second);
    }
}

BindedParseCallback<ParserCommand*> CommandParser::bindedParse(const std::string &input) {
    ParseResult result = this->parse(input);
    return std::bind(result.first, result.second);
}

std::pair<ParseCallback<ParserCommand *>, ParsedOptions> CommandParser::getOptionsError(
    ParserCommandInfo<ParserCommand *> command, ParsedOptions arguments
) {
    ParseCallback<ParserCommand *> protectedDisplayError = command.getErrorDisplay()
        ? std::bind(&CommandParser::printArgumentsError, this, command, std::placeholders::_1)
        : throw std::invalid_argument("Arguments validation failed. You can get better error message by providing displayError callback");

    return std::make_pair(protectedDisplayError, arguments);
}

std::pair<ParseCallback<ParserCommand *>, ParsedOptions> CommandParser::getCommandError() {
    ParseCallback<ParserCommand *> commandNotFound = this->displayError
        ? TypesHelper::methodToFunction(&CommandParser::printCommandsError, this)
        : throw std::invalid_argument("Command not found. You can get better error message by providing displayError callback");

    return std::make_pair(commandNotFound, ParsedOptions());
}
