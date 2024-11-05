#include "TypesHelper.h"

#include <iostream>

std::pair<int, int> TypesHelper::cell(const std::string &coord) {
    std::smatch groups;
    std::regex_match(coord, groups, std::regex("^(\\d+),(\\d+)$"));
    return {std::stoi(groups[1].str()), std::stoi(groups[2].str())};
}

