#include "ParserParameterBuilder.h"

void ParserParameterBuilder::reset() {
    flags.clear();
    validator = ".";
    description = "";
    necessary = false;
}
