#include "CommandBuildDirector.h"

CommandBuildDirector::CommandBuildDirector(ParserCommandBuilder *commandBuilder)
    : commandBuilder(*commandBuilder)
{}

ParserCommandBuilder & CommandBuildDirector::buildBasicCommand(ParseCallback function, std::string description) const {
    commandBuilder.setDescription(std::move(description));
    commandBuilder.setCallback(std::move(function));

    return commandBuilder;
}

void CommandBuildDirector::reset() const {
    commandBuilder.reset();
}
