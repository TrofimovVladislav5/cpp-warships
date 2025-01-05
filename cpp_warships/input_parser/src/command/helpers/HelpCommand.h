#pragma once
#include "../../Parser.h"
#include "../../ParserCommand.h"


class HelpCommand : public ParserCommand {
private:
    SchemeMap<ParserCommand*> scheme;
public:
    explicit HelpCommand(SchemeMap<ParserCommand*> scheme);

    void execute(ParsedOptions options) override;
};
