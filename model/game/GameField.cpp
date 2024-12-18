#include "GameField.h"

#include <iostream>
#include "Structures.h"
#include <unordered_map>
#include <unordered_set>

GameField::GameField(int width, int height)
    : width(width)
    , height(height)
{}

GameField::GameField(const GameField& other)
    : width(other.getWidth())
    , height(other.getHeight())
    , shipsCoordinateMap(other.getShipsCoordinateMap())
    , attacksOnField(other.getAttacksOnField())
{}

GameField::GameField(GameField&& other)
    : width(other.getWidth())
    , height(other.getHeight())
    , shipsCoordinateMap(std::move(other.getShipsCoordinateMap()))
    , attacksOnField(std::move(other.getAttacksOnField()))
{}

GameField& GameField::operator=(const GameField& other) {
    if (this != &other) {
        this->width = other.getWidth();
        this->height = other.getHeight();
        this->shipsCoordinateMap = other.getShipsCoordinateMap();
        this->attacksOnField = other.getAttacksOnField();
    }

    return *this;
}

GameField& GameField::operator=(GameField&& other) {
    if (this != &other) {
        this->width = other.getWidth();
        this->height = other.getHeight();
        this->shipsCoordinateMap = other.getShipsCoordinateMap();
        this->attacksOnField = other.getAttacksOnField();
    }

    return *this;
}

GameField::GameField(int width, int height,
    std::unordered_map<Ship *, std::unordered_set<std::pair<int, int>, hashFunc> > shipsCoordinateMap,
    std::unordered_set<std::pair<int, int>, hashFunc> attacksOnField
)
    : width(width)
    , height(height)
    , shipsCoordinateMap(shipsCoordinateMap)
    , attacksOnField(attacksOnField)
{}

int GameField::getHeight() const {
    return height;
}

int GameField::getWidth() const {
    return width;
}

const std::unordered_map<Ship*, std::unordered_set<std::pair<int, int>,hashFunc>>& GameField::getShipsCoordinateMap() const {
    return this->shipsCoordinateMap;
}

const std::unordered_set<std::pair<int, int>, hashFunc>& GameField::getAttacksOnField() const {
    return this->attacksOnField;
}

bool GameField::canPlaceShip(std::pair<int, int> initialCoordinate, Direction direction, int length) {
    if (!shipCoordinatesInField(initialCoordinate, length, direction) || intersectionShips(initialCoordinate, length, direction)) {
        return false;
    }
    return true;
}

void GameField::placeShip(Ship* ship, std::pair<int, int> initialCoordinate, Direction direction) {
    int length = ship->getLength();
    for (int i = 0; i < length;i++) {
        std::pair<int, int> newCoordinate = initialCoordinate;
        if (direction == Direction::horizontal) newCoordinate.first += i;
        else newCoordinate.second += i;

        this->shipsCoordinateMap[ship].insert(newCoordinate);
    }
}

bool GameField::intersectsWithArea(std::pair<int, int> center, int radius) {
    for (int dy = -radius; dy <= radius; ++dy) {
        for (int dx = -radius; dx <= radius; ++dx) {
            int newX = center.first + dx;
            int newY = center.second + dy;
            if (newX >= 0 && newX < width && newY >= 0 && newY < height) {
                std::pair<int, int> checkCoord = {newX, newY};
                for (const auto& [ship, coordinates] : shipsCoordinateMap) {
                    if (coordinates.find(checkCoord) != coordinates.end()) {
                        return true;
                    }
                }
            }
        }
    }

    return false;
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


AttackResult GameField::attack(std::pair<int, int> initialCoordinate, int damageCount) {
    if (initialCoordinate.first < 0 || initialCoordinate.first >= width
    ||  initialCoordinate.second < 0 || initialCoordinate.second >= height) throw std::out_of_range("Invalid coordinates to attack");

    attacksOnField.insert(initialCoordinate);
    for (const auto& [ship, coordinates] : shipsCoordinateMap) {
        if (auto it = coordinates.find(initialCoordinate); it != coordinates.end()) {
            int index = std::distance(coordinates.begin(),it);
            ship->takeDamage(index, damageCount);
            return (ship->isDestroyed()) ? AttackResult::destroyed : AttackResult::damaged;
        }
    }

    return AttackResult::miss;
}

std::pair<int, int> GameField::removeShip(const std::pair<int, int> &coordinate) {
    int index = 0;
    for (const auto& [ship, coordinates] : shipsCoordinateMap) {
        if (coordinates.find(coordinate) != coordinates.end()) {
            int size = ship->getLength();
            shipsCoordinateMap.erase(ship);
            return {index, size};
        }
        index++;
    }
    return {-1, -1};
}

void GameField::clear() {
    this->shipsCoordinateMap.clear();
    this->attacksOnField.clear();
}

bool GameField::isAllShipsDestroyed() const {
    for (const auto& [ship, coordinates] : shipsCoordinateMap) {
        if (!ship->isDestroyed()) {
            return false;
        }
    }
    return true;
}

void GameField::updateShipsCoordinateMap(const std::unordered_map<Ship *, std::unordered_set<std::pair<int, int>, hashFunc> > &newMap) {
    shipsCoordinateMap.clear();
    shipsCoordinateMap = newMap;
}
