#pragma once
#include <string>

#include "input-reader/InputReader.h"

class ViewHelper {
public:
    static void consoleOut(const std::string& output, int level = 0);
    static void errorOut(const std::string& output);
    static void errorOut(const std::string& output, const std::exception& e);
    static bool confirmAction(InputReader<>* reader, const std::string& confirmMessage);
};
