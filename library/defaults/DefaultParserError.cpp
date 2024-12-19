#include "DefaultParserError.h"

#include "library/ViewHelper.h"

void DefaultParserError::CommandNotFoundError (ParsedOptions options) {
    ViewHelper::consoleOut("\nRelated command not found. Please, try again \n");
}

void DefaultParserError::WrongFlagValueError(ParsedOptions options) {
    ViewHelper::consoleOut("\nUnresolved combination of command and flag. \n "
                           "It probably means that flag that you have entered either not listed in specification or \n"
                           "the value you passed doesn't pass the validation tests \n");
}