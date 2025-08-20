#include "../../include/command/HelpCommand.h"
#include "../../include/DefaultHelp.h"

#include <cpp_warships/utilities/include/ViewHelper.h>

namespace cpp_warships::input_parser {
    HelpCommand::HelpCommand(SchemeMap<ParserCommand*> scheme)
        : ParserCommand()
        , scheme(std::move(scheme))
    {}

    void HelpCommand::execute(ParsedOptions options) {
        ViewHelper::consoleOut("This is the list of supported commands:");

        for (const auto& command : scheme) {
            auto commandPrint = command.second.getPrintHelp();

            if (commandPrint) {
                commandPrint(options);
            } else {
                DefaultHelp::PrintCommand<ParserCommand*>(command, DefaultHelp::PrintParam);
                ViewHelper::consoleOut("");
            }
        }
    }
}