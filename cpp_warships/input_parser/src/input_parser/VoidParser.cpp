#include <input_parser/DefaultHelp.h>
#include <input_parser/VoidParser.h>
#include <input_parser/builder/ConfigCommandBuilder.h>
#include <input_parser/model/Parser.h>
#include <input_parser/model/ParserCommandInfo.h>
#include <utilities/TypesHelper.h>
#include <utilities/ViewHelper.h>

#include <stdexcept>

namespace cpp_warships::input_parser {
    void VoidParser::printCommandsHelp(model::ParsedOptions options) {
        ViewHelper::consoleOut("This is the list of supported commands:");

        for (const auto& command : scheme) {
            auto commandPrint = command.second.getPrintHelp();
            if (commandPrint) {
                commandPrint(options);
            } else {
                DefaultHelp::PrintCommand<void>(command, DefaultHelp::PrintParam);
                ViewHelper::consoleOut("");
            }
        }
    }

    VoidParser::VoidParser(const model::SchemeMap<void>& scheme)
        : VoidParser(scheme, nullptr, nullptr) {}

    VoidParser::VoidParser(
        const model::SchemeMap<void>& scheme,
        const model::ParseCallback<void>& displayError,
        const model::SchemeHelpCallback<void>& printHelp
    )
        : model::Parser<void>(scheme, displayError, printHelp) {
        if (scheme.find("help") == scheme.end()) {
            builder::ConfigCommandBuilder<void> commandBuilder;
            model::ParserCommandInfo<void>* helpInfo;

            if (printHelp) {
                helpInfo = new model::ParserCommandInfo<void>(
                    {commandBuilder.setDescription("command::Command to display this message")
                         .setCallback(std::bind(printHelp, scheme))
                         .buildAndReset()}
                );
            } else {
                helpInfo = new model::ParserCommandInfo<void>(
                    {commandBuilder.setDescription("command::Command to display this message")
                         .setCallback(
                             TypesHelper::methodToFunction(&VoidParser::printCommandsHelp, this)
                         )
                         .buildAndReset()}
                );
            }

            this->scheme.insert({"help", *helpInfo});
            delete helpInfo;
        }
    }

    model::BindedParseCallback<void> VoidParser::bindedParse(const std::string& input) {
        std::pair<model::ParseCallback<void>, model::ParsedOptions> result = this->parse(input);
        return std::bind(result.first, result.second);
    }

    void VoidParser::executedParse(const std::string& input) {
        std::pair<model::ParseCallback<void>, model::ParsedOptions> result = this->parse(input);
        result.first(result.second);
    }

    std::pair<model::ParseCallback<void>, model::ParsedOptions> VoidParser::getCommandError() {
        model::ParseCallback<void> commandNotFound =
            this->displayError ? this->displayError
                               : throw std::invalid_argument(
                                     "command::Command not found. You can get better error "
                                     "message by providing displayError callback"
                                 );

        return std::make_pair(commandNotFound, model::ParsedOptions());
    }

    std::pair<model::ParseCallback<void>, model::ParsedOptions> VoidParser::getOptionsError(
        model::ParserCommandInfo<void> command,
        model::ParsedOptions arguments
    ) {
        model::ParseCallback<void> protectedDisplayError =
            command.getErrorDisplay() ? command.getErrorDisplay()
                                      : throw std::invalid_argument(
                                            "Arguments validation failed. You can "
                                            "get better error message "
                                            "by providing displayError callback"
                                        );

        return std::make_pair(protectedDisplayError, arguments);
    }
}  // namespace cpp_warships::input_parser