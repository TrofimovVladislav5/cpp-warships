#include "StringHelper.h"

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