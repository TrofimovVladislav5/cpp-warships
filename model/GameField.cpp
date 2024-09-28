#include "GameField.h"

#include <iostream>

#include "Ship.h"
#include <vector>

GameField::GameField(int width, int height){
    this->height = height;
    this->width = width;
    this->field = std::vector<std::vector<CellField>>(
        height,
        std::vector<CellField>(width, CellField::UNKNOWN)
    );
}

bool GameField::placeShip(Ship* ship, int x, int y){
    int length = ship->getLength();
    Direction direct = ship->getDirection();
    if (!placeShipValidate(x, y, length, direct) || intersectionShips(x, y, length, direct)){
        std::cout << "Can't place ship" << std::endl;
        return false;
    }
    for (int i = 0; i < length; i++){
        if (direct == Direction::VERTICAL){
            field[y + i][x] = CellField::SHIP;
        } else {
            field[y][x + i] = CellField::SHIP;
        }
    }
    return true;
}
bool GameField::placeShipValidate(int x, int y, int length, Direction direct) const {
    if (direct == Direction::VERTICAL) {
        return y + length <= height;
    } else {
        return x + length <= width;
    }
}
bool GameField::intersectionShips(int x, int y, int length, Direction direct){
    for (int i = 0; i < length; i++){
        int newX = x;
        int newY = y;
        if (direct == Direction::VERTICAL){
            newY = y + i;
        }
        if (direct == Direction::HORIZONTAL){
            newX = x + i;
        }
        if (field[newY][newX] != CellField::EMPTY || neighborShips(newX, newY)){
            return true;
        }
    }
    return false;
}
bool GameField::neighborShips(int x, int y) const {
    for (int dy = -1; dy <= 1; dy++){
        for (int dx = -1; dx <= 1; dx++){
            int newX = x + dx;
            int newY = y + dy;
            if (newX >= 0 && newX < width && newY >= 0 && newY < height){
                if (field[newY][newX] == CellField::SHIP){
                    return true;
                }
            }
        }
    }
    return false;
}
