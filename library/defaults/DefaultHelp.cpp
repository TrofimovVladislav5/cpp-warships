#include "DefaultHelp.h"

#include "library/ViewHelper.h"
#include "library/parser/ParserCommandInfo.h"

void DefaultHelp::PrintParam(const ParserParameter &param) {
    std::string necessary = param.getNecessary() ? "true" : "false";
    ViewHelper::consoleOut("is necessary: " + necessary, 3);
    ViewHelper::consoleOut("description: " + param.getDescription(), 3);
    std::string flagsOutput;
    for (const auto& flag : param.getFlags()) {
        flagsOutput += flag + " ";
    }

    ViewHelper::consoleOut("flags: [ " + flagsOutput + "]", 3);
}

void DefaultHelp::PrintCommand(const std::pair<std::string, ParserCommandInfo> &command,
    const std::function<void(ParserParameter)> &printParam) {
    ParserCommandInfo currentCommand = command.second;
    ViewHelper::consoleOut(command.first + ": ", 1);
    ViewHelper::consoleOut("description: " + currentCommand.getDescription(), 2);
    std::string params = currentCommand.getParams().empty() ? "empty" : "";
    ViewHelper::consoleOut("params: " + params, 2);

    for (const auto& param : currentCommand.getParams()) {
        printParam(param);
    }
}