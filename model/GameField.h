#pragma once

#include <vector>

#include "Ship.h"

class GameField {
private:
    int height;
    int width;
    std::vector<std::vector<CellField>> field;
public:
    GameField(int width, int height);
    void printGameField();
    bool placeShip(Ship* ship, int x, int y);
    bool placeShipValidate(int x, int y, int length, Direction direct) const;
    bool intersectionShips(int x, int y, int length, Direction direct);
    bool neighborShips(int x, int y) const;
};
