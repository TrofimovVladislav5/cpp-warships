#include "ConsoleInputReader.h"

#include <iostream>

std::string ConsoleInputReader::readCommand() {
    std::string command;
    std::getline(std::cin, command);
    return command;
}
