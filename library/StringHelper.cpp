#include "StringHelper.h"

#include <stdexcept>

// template<typename T>
std::vector<std::string> StringHelper::split(const std::string &initial, char delim) {
    std::vector<std::string> elems;
    std::string current;

    for (int i = 0; i < initial.length(); i++) {
        if (initial[i] == delim) {
            elems.push_back(current);
            current = "";
        } else {
            current += initial[i];
        }
    }

    if (!current.empty()) {
        elems.push_back(current);
    }

    return elems;
}

const std::string &StringHelper::patternCoordinate(int fieldSize) {
    std::string pattern;
    if (fieldSize < 10 || fieldSize > 26) {
        throw std::invalid_argument("Method functions support field size from 10 to 25");
    }
    if (fieldSize < 20) {
        pattern = "^([0-9]|1[0-" + std::to_string(fieldSize % 10) + "])\\,([0-9]|1[0-" + std::to_string(fieldSize % 10) + "])$";
    } else if (fieldSize - 1 < 26) {
        pattern = "^([0-9]|1[0-9]|2[0-" + std::to_string(fieldSize % 10) + "])\\,([0-9]|1[0-9]|2[0-" + std::to_string(fieldSize % 10) + "])$";
    }
    return pattern;
}
