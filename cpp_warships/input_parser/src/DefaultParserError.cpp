#include "../include/DefaultParserError.h"

#include <cpp_warships/utilities/include/ViewHelper.h>


namespace cpp_warships::input_parser {
    void DefaultParserError::CommandNotFoundError(ParsedOptions options) {
        ViewHelper::consoleOut("\nRelated command not found. Please, try again \n");
    }

    void DefaultParserError::WrongFlagValueError(ParsedOptions options) {
        ViewHelper::consoleOut(
                "\nUnresolved combination of command and flag. \n "
                "It probably means that flag that you have entered either not listed in specification "
                "or \n"
                "the value you passed doesn't pass the validation tests \n");
    }
}