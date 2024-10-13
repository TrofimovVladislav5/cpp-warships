#include "ParserCommandBuilder.h"

void ParserCommandBuilder::reset() {
    description = "";
    parameters.clear();
    executable = nullptr;
    displayError = nullptr;
}
