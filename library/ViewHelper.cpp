#include "ViewHelper.h"

#include <iostream>
#include <ostream>

void ViewHelper::consoleOut(const std::string &output, int level) {
    std::string tabs = std::string(level, '\t');
    std::cout << tabs << output << std::endl;
}

void ViewHelper::errorOut(const std::string &output) {
    std::cerr << output << std::endl;
}
