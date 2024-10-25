#include "GameField.h"

#include <iostream>

#include "Structures.h"
#include <unordered_map>
#include <unordered_set>

bool GameField::validateCoordinates(std::pair<int, int> coordToCheck) {
    return coordToCheck.first > 0 && coordToCheck.first < width && coordToCheck.second > 0 && coordToCheck.second < height;
}

GameField::GameField(int width, int height)
    : width(width)
    , height(height)
{}

GameField::GameField(const GameField& other)
    : width(other.getWidth())
    , height(other.getHeight())
    , shipsCoordinateMap(other.getShipsCoordinateMap())
    , attackCoordinateMap(other.getAttackCoordinateMap())
{}

GameField::GameField(GameField&& other)
    : width(other.getWidth())
    , height(other.getHeight())
    , shipsCoordinateMap(std::move(other.getShipsCoordinateMap()))
    , attackCoordinateMap(std::move(other.getAttackCoordinateMap()))
{}

GameField& GameField::operator=(const GameField& other) {
    if (this != &other) {
        this->width = other.getWidth();
        this->height = other.getHeight();
        this->shipsCoordinateMap = other.getShipsCoordinateMap();
        this->attackCoordinateMap = other.getAttackCoordinateMap();
    }

    return *this;
}

GameField& GameField::operator=(GameField&& other) {
    if (this != &other) {
        this->width = other.getWidth();
        this->height = other.getHeight();
        this->shipsCoordinateMap = other.getShipsCoordinateMap();
        this->attackCoordinateMap = other.getAttackCoordinateMap();
    }

    return *this;
}

int GameField::getHeight() const {
    return height;
}

int GameField::getWidth() const {
    return width;
}

const std::unordered_map<Ship*, std::unordered_set<std::pair<int, int>,hashFunc>>& GameField::getShipsCoordinateMap() const {
    return this->shipsCoordinateMap;
}

const std::unordered_set<std::pair<int, int>, hashFunc>& GameField::getAttackCoordinateMap() const {
    return this->attackCoordinateMap;
}

bool GameField::placeShip(Ship* ship, std::pair<int, int> initialCoordinate, Direction direction) {
    int length = ship->getLength();
    if (!shipCoordinatesInField(initialCoordinate, length, direction) || intersectionShips(initialCoordinate, length, direction)) {
        std::cout << "Can't place ship" << std::endl;
        return false;
    }

    for (int i = 0; i < length;i++) {
        std::pair<int, int> newCoordinate = initialCoordinate;
        if (direction == Direction::horizontal) newCoordinate.first += i;
        else newCoordinate.second += i;

        this->shipsCoordinateMap[ship].insert(newCoordinate);
    }

    return true;
}

bool GameField::shipCoordinatesInField(std::pair<int, int> coords, int length, Direction direction) const {
    if (direction == Direction::horizontal) {
        return coords.first + length <= width;
    }

    return coords.second + length <= height;
}

bool GameField::shipsAreContacting(std::pair<int, int> coords) const {
    for (int dy = -1;dy <= 1;dy++){
        for (int dx = -1;dx <= 1;dx++){
            int newX = coords.first + dx;
            int newY = coords.second + dy;
            if (newX >= 0 && newX < width && newY >= 0 && newY < height){
                std::pair<int, int> neighborCoords = {newX, newY};
                for (const auto& [ship, coordinates] : shipsCoordinateMap){
                    if (coordinates.find(neighborCoords) != coordinates.end()){
                        return true;
                        }
                    }
                }
            }
        }
    return false;
}

bool GameField::intersectionShips(std::pair<int , int> coordinates, int length, Direction direction) const {
    for (int i = 0;i < length;i++){
        std::pair<int, int> tempCoordinates = coordinates;
        if (direction == Direction::horizontal) {
            tempCoordinates.first += i;
            }
        else if (direction == Direction::vertical){
            tempCoordinates.second += i;
            }

        for (const auto& [ship, coords] : shipsCoordinateMap) {
            if (coords.find(tempCoordinates) != coords.end()) {
                return true;
                }
            }

        if (shipsAreContacting(tempCoordinates)) return true;
        }
    return false;
}


bool GameField::attack(std::pair<int, int> initialCoordinate, int damageCount) {
    if (initialCoordinate.first < 0 || initialCoordinate.first >= width
    ||  initialCoordinate.second < 0 || initialCoordinate.second >= height) throw std::out_of_range("Invalid coordinates to attack");


    for (const auto& [ship, coordinates] : shipsCoordinateMap) {
        if (auto it = coordinates.find(initialCoordinate); it != coordinates.end()) {
            int index = std::distance(coordinates.begin(),it);
            ship->takeDamage(index, damageCount);
            attackCoordinateMap.insert(*it);
            return true;
        }
    }
    return false;
}

int GameField::removeShip(const std::pair<int, int> &coordinate) {
    int index = 0;
    for (const auto& [ship, coordinates] : shipsCoordinateMap) {
        if (coordinates.find(coordinate) != coordinates.end()) {
            shipsCoordinateMap.erase(ship);
            return index;
        }
        index++;
    }
    return -1;
}