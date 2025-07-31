#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Structures.h"
#include "Ship.h"

namespace cpp_warships::application {

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
        GameField(int width, int height,
            std::unordered_map<Ship*, std::unordered_set<std::pair<int, int>,hashFunc>> shipsCoordinateMap,
            std::unordered_set<std::pair<int, int>, hashFunc> attacksOnField
        );
        int getHeight() const;
        int getWidth() const;

        [[nodiscard]] const std::unordered_map<Ship*, std::unordered_set<std::pair<int, int>, hashFunc>>& getShipsCoordinateMap() const;
        [[nodiscard]] const std::unordered_set<std::pair<int, int>, hashFunc>& getAttacksOnField () const;
        void updateShipsCoordinateMap(const std::unordered_map<Ship*, std::unordered_set<std::pair<int, int>, hashFunc>>& newMap);
        bool canPlaceShip(std::pair<int, int> initialCoordinate, Direction direction, int length);
        void placeShip(Ship* ship, std::pair<int, int> initialCoordinate, Direction direction);
        bool intersectsWithArea(std::pair<int, int> center, int radius);
        std::pair<int, int> removeShip(const std::pair<int, int>& coordinate);

        AttackResult attack(std::pair<int, int> initialCoordinate, int damageCount);
        bool isAllShipsDestroyed() const;
        void clear();
    };
} // namespace cpp_warships::application