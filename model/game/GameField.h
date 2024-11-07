#pragma once
#include "Structures.h"
#include "Ship.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>

class GameField {
private:
    int width;
    int height;
    std::unordered_map<Ship*, std::unordered_set<std::pair<int, int>,hashFunc>> shipsCoordinateMap;
    std::unordered_set<std::pair<int, int>, hashFunc> attacksOnField;
    bool shipCoordinatesInField(std::pair<int, int> coords, int length, Direction direction) const;
    bool shipsAreContacting(std::pair<int, int> coords) const;
    bool intersectionShips(std::pair<int , int> coordinates, int length, Direction direction) const;
public:
    GameField(int width, int height);
    GameField(const GameField& other);
    GameField(GameField&& other);
    GameField& operator=(const GameField& other);
    GameField& operator=(GameField&& other);

    int getHeight() const;
    int getWidth() const;

    const std::unordered_map<Ship*, std::unordered_set<std::pair<int, int>, hashFunc>>& getShipsCoordinateMap() const;
    const std::unordered_set<std::pair<int, int>, hashFunc>& getAttacksOnField () const;

    bool canPlaceShip(std::pair<int, int> initialCoordinate, Direction direction, int length);
    void placeShip(Ship* ship, std::pair<int, int> initialCoordinate, Direction direction);
    std::pair<int, int> removeShip(const std::pair<int, int>& coordinate);

    AttackResult attack(std::pair<int, int> initialCoordinate, int damageCount);
    void clear();
};