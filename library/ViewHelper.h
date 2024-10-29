#pragma once
#include <string>

class ViewHelper {
public:
    static void consoleOut(const std::string &output, int level = 0);
    static void errorOut(const std::string& output);
};
