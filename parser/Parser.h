#pragma once
#include <regex>
#include <iostream>
#include "model/Structures.h"

class Parser {
private:
    bool parseNumber(std::string string_number);
    bool parseCoordinate(const std::string& stringCoordinates, std::smatch& groups);
public:
    std::pair<int, int> parseCoordinateCell();
    int parseLengthShip();
    Direction parseDirectionShip();
};