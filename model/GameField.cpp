#include <iostream>
#include <vector>
#include "GameField.h"
#include "Ship.h"
#include "Structures.h"

GameField::GameField(int width, int height)
    : width(width)
    , height(height)
    , field(std::vector<std::vector<Cell>>(height, std::vector<Cell>(width, Cell::empty)))
{}

bool GameField::placeShip(Ship* ship, std::pair<int, int> initialCoordinate, Direction direction) {
    int length = ship->getLength();
    if (!shipCoordinatesInField(initialCoordinate, length, direction) || intersectionShips(initialCoordinate, length, direction)) {
        std::cout << "Can't place ship" << std::endl;
        return false;
    }

    for (int i = 0; i < length;i++) {
        std::pair<int, int> newCoordinate = initialCoordinate;
        if (direction == Direction::horizontal) {
            newCoordinate.first += i;
        }
        else {
            newCoordinate.second += i;
        }
        shipsCoordinateMap[ship].insert(newCoordinate);
    }

    return true;
}

bool GameField::shipCoordinatesInField(std::pair<int, int> coords, int length, Direction direction) const {
    if (direction == Direction::horizontal) {
        return coords.first + length <= width;
    }
    else {
        return coords.second + length <= height;
    }
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

void GameField::showField() {
    for (int y = 0; y < height;y++) {
        for (int x = 0; x < width;x++) {
            field[y][x] = Cell::unknown;
        }
    }

    for (const auto& [ship, coordinates] : shipsCoordinateMap) {
        int index = 0;
        for (const auto& coord : coordinates) {
            SegmentState segmentState = ship->getSegment(index);
            if (segmentState == SegmentState::intact) {
                field[coord.second][coord.first] = Cell::ship_int;
            } else if (segmentState == SegmentState::damaged) {
                field[coord.second][coord.first] = Cell::ship_damaged;
            } else if (segmentState == SegmentState::destroyed) {
                field[coord.second][coord.first] = Cell::ship_destroyed;
            }
            index++;
        }
    }

    scanCellsNextToDestroyedShip(field, shipsCoordinateMap);

    std::string result;
    std::string upperBar = "№ ";
    for (int x = 0; x < width; x++) {
        upperBar += std::to_string(x) + " ";
    }
    std::cout << upperBar << std::endl;
    for (int y = 0; y < height;y++) {
        result.clear();
        result += std::to_string(y) + " ";
        for (int x = 0; x < width;x++) {
            switch (field[y][x]){
                case Cell::empty:
                    result += "+ ";
                    break;
                case Cell::ship_int:
                    result += "2 ";
                    break;
                case Cell::ship_damaged:
                    result += "1 ";
                    break;
                case Cell::ship_destroyed:
                    result += "0 ";
                    break;
                case Cell::unknown:
                    result += "* ";
                    break;
            }
        }
        std::cout << result << std::endl;
    }
}


bool GameField::attack(std::pair<int, int> initialCoordinate, int damageCount) {
    if (initialCoordinate.first < 0 || initialCoordinate.first >= width 
    ||  initialCoordinate.second < 0 || initialCoordinate.second >= height) throw std::out_of_range("Invalid coordinates to attack");


    for (const auto& [ship, coordinate] : shipsCoordinateMap) {
        int index = 0;
        for (auto& coord : coordinate) {
            if (coord == initialCoordinate) {
                SegmentState currentState = ship->getSegment(index);
                ship->takeDamage(index, damageCount);
                attackCoordinateMap.insert(coord);
                return true;
            }
            index++;
        }
    }
    return false;
}

bool GameField::validateCoordinates(std::pair<int, int> coordToCheck) {
    return coordToCheck.first > 0 && coordToCheck.first < width 
        && coordToCheck.second > 0 && coordToCheck.second < height;
}

void GameField::scanCellsNextToDestroyedShip(std::vector<std::vector<Cell>>& field, std::unordered_map<Ship*, std::unordered_set<std::pair<int, int>,hashFunc>>& shipsCoordinateMap) {
    for (const auto& [ship, coordinate] : shipsCoordinateMap) {
        if (ship->isDestroyed()) {
            std::pair<int, int> minCoord = *coordinate.begin();
            std::pair<int, int> maxCoord = *coordinate.begin();
            for (const auto& coord : coordinate) {
                if (coord.first < minCoord.first || (coord.first == minCoord.first && coord.second < minCoord.second)) {
                    minCoord = coord;
                }

                if (coord.first > maxCoord.first || (coord.first == maxCoord.first && coord.second > maxCoord.second)) {
                    maxCoord = coord;
                }
            }
            std::pair<int, int> shiftMinCoord = std::make_pair(minCoord.first - 1, minCoord.second - 1);
            std::pair<int, int> shiftMaxCoord = std::make_pair(maxCoord.first + 1, maxCoord.second + 1);
            if (validateCoordinates(shiftMaxCoord) && validateCoordinates(shiftMinCoord)) {
                for (int y = shiftMinCoord.second;y <= shiftMaxCoord.second;y++) {
                    for (int x = shiftMinCoord.first;x <= shiftMaxCoord.first;x++) {
                        if (minCoord.first <= x && x <= maxCoord.first && minCoord.second <= y && y <= maxCoord.second) {
                                continue;
                            }
                            field[y][x] = Cell::empty;
                        }
                    }
                }

            else if (validateCoordinates(shiftMaxCoord) && !validateCoordinates(shiftMinCoord)) {
                for (int y = minCoord.second;y <= shiftMaxCoord.second;y++) {
                    for (int x = minCoord.first;x <= shiftMaxCoord.first;x++) {
                        if (minCoord.first <= x && x <= maxCoord.first && minCoord.second <= y && y <= maxCoord.second) {
                            continue;
                        }
                        field[y][x] = Cell::empty;
                    }
                }
            }

            else if (!validateCoordinates(shiftMaxCoord) && validateCoordinates(shiftMinCoord)) {
                for (int y = shiftMinCoord.second;y <= maxCoord.second;y++) {
                    for (int x = shiftMinCoord.first;x <= maxCoord.first;x++) {
                        if (minCoord.first <= x && x <= maxCoord.first && minCoord.second <= y && y <= maxCoord.second) {
                            continue;
                        }
                        field[y][x] = Cell::empty;
                    }
                }
            }
        }
    }
}