#pragma once
#include <vector>
#include <string>
// template<typename T>
class StringHelper {
public:
    static std::vector<std::string> split(const std::string &initial, char delim);
    static const std::string& patternCoordinate(int fieldSize);
};
