#include <iostream>
#include <vector>
#include "GameField.h"
#include "Ship.h"
#include "Structures.h"


GameField::GameField(int width, int height)
    : width(width)
    , height(height)
    , field(std::vector<std::vector<CellField>>(height, std::vector<CellField>(width, CellField::EMPTY)))
{}

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

bool GameField::removeShip(int x, int y){
    if (x >= width || x >= height || x < 0 || y < 0){
        return true;
    }
    if (field[y][x] != CellField::SHIP){
        return true;
    }

    Direction direction;
    int minimalShipLength = 1;

    if (x + 1 < width && field[y][x + 1] == CellField::SHIP) {
        direction = Direction::HORIZONTAL;

        for (int i = x + 1;i < width && field[y][i] == CellField::SHIP;i++) {
            minimalShipLength++;
        }    
    }
    else if (y + 1 < height && field[y + 1][x] == CellField::SHIP){
        direction = Direction::VERTICAL;
        for (int i = y + 1;i < height && field[i][x] == CellField::SHIP;i++) {
            minimalShipLength++;
        }
    }
    else{
        direction = Direction::HORIZONTAL;
    }

    for (int i = 0;i < minimalShipLength;i++){
        if (direction == Direction::HORIZONTAL) {
            field[y][x + i] = CellField::EMPTY;
        }
        else if (direction == Direction::VERTICAL) {
            field[y + i][x] = CellField::EMPTY;
        }
    }
}

int GameField::getHeight() const {return height;}
int GameField::getWidth() const {return width;}

void GameField::printGameField(){
    std::string mainField;
    std::string fieldHorizontal;
    std::string indent = "  ";
    for (int y = 0;y < height;y++){
        fieldHorizontal += std::to_string(y) + " ";
    }
    std::cout << indent + fieldHorizontal << std::endl;
    for (int y = 0;y < height;y++){
        mainField.clear();
        mainField = std::to_string(y) + " ";
        for (int x = 0;x < width;x++){
            switch (field[y][x])
            {
            case CellField::UNKNOWN:
                mainField += "U ";
                break;
            case CellField::EMPTY:
                mainField += "_ ";
                break;
            case CellField::SHIP:
                mainField += "S ";
                break;
            default:
                break;
            }
        }
        std::cout << mainField << std::endl;
    }
}

bool GameField::attack(int x, int y, int damageCount){
    return true;
}