#include "DefaultHelp.h"

#include "library/ViewHelper.h"
#include "library/parser/ParserCommandInfo.h"

void DefaultHelp::PrintParam(const ParserParameter &param) {
    std::string flagsOutput;
    for (const auto& flag : param.getFlags()) {
        flagsOutput += flag + " ";
    }
    ViewHelper::consoleOut("├── flags: [ " + flagsOutput + "]", 2);

    std::string description = param.getDescription().empty() ? "none" : param.getDescription();
    ViewHelper::consoleOut("├── description: " + description, 2);
    std::string necessary = param.getNecessary() ? "true" : "false";
    ViewHelper::consoleOut("└── is necessary: " + necessary, 2);
}

void DefaultHelp::PrintCommand(
    const std::pair<std::string, ParserCommandInfo> &command,
    const std::function<void(ParserParameter)> &printParam
) {
    ParserCommandInfo currentCommand = command.second;
    ViewHelper::consoleOut("print '" + command.first + "': ", 1);
    ViewHelper::consoleOut("├── description: " + currentCommand.getDescription(), 1);

    std::vector<ParserParameter> params = currentCommand.getParams();
    if (params.empty()) {
        ViewHelper::consoleOut("└── params: empty", 1);
    } else {
        ViewHelper::consoleOut("└── params:", 1);
    }

    for (int i = 0; i < params.size(); i++) {
        ViewHelper::consoleOut("Param (" + std::to_string(i + 1) + ")", 2);
        printParam(params[i]);
    }
}