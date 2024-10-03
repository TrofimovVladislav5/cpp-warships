#pragma once

#include <vector>
#include "Ship.h"
#include "Structures.h"

class GameField {
private:
    int height;
    int width;
    std::vector<std::vector<CellField>> field;
    bool placeShipValidate(int x, int y, int length, Direction direct) const;
    bool intersectionShips(int x, int y, int length, Direction direct);
    bool neighborShips(int x, int y) const;
public:
    GameField(int width, int height);
    int getHeight() const;
    int getWidth() const;
    void printGameField();
    bool placeShip(Ship* ship, int x, int y);
    bool removeShip(int x, int y);
    bool attack(int x, int y, int damageCount);
};
