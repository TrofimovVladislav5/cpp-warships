#pragma once
#include <string>
#include <vector>


namespace cpp_warships::utilities {
    class StringHelper {
    public:
        static std::vector<std::string> split(const std::string &initial, char delim);
        static std::string patternCoordinate(int fieldSize);
        static std::string toLower(const std::string &str);
        static std::string trim(const std::string &str);
    };
}