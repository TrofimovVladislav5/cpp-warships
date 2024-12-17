#include "ViewHelper.h"

#include <iostream>
#include <ostream>

#include "StringHelper.h"

void ViewHelper::consoleOut(const std::string &output, int level) {
    std::string tabs = std::string(level, '\t');
    std::cout << tabs << output << std::endl;
}

void ViewHelper::errorOut(const std::string &output) {
    std::cerr << output << std::endl;
}

void ViewHelper::errorOut(const std::string &output, const std::exception &e) {
    errorOut(output);
    errorOut(e.what());
}

bool ViewHelper::confirmAction(const std::string &confirmMessage) {
    std::string input;
    std::getline(std::cin, input);

    std::string inputLower = StringHelper::toLower(input);
    std::string confirmMessageLower = StringHelper::toLower(confirmMessage);
    return inputLower == confirmMessageLower;
}
