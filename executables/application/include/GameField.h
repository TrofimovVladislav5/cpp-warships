#pragma once

#include <cpp_warships/game_saves/include/ISerializable.h>
#include <unordered_map>
#include <unordered_set>

#include "Ship.h"
#include "Structures.h"

namespace cpp_warships::application {
    inline char GameFieldName[] = "GameField";

    using ShipCoordinatesSet = std::unordered_set<std::pair<int, int>, hashFunc>;
    using ShipsField = std::unordered_map<Ship*, ShipCoordinatesSet>;

    class GameField
        : public game_saves::ISerializable<GameFieldName>
    {
    private:
        int width;
        int height;
        std::unordered_map<Ship*, std::unordered_set<std::pair<int, int>,hashFunc>> shipsCoordinateMap;
        std::unordered_set<std::pair<int, int>, hashFunc> attacksOnField;

        [[nodiscard]] bool shipCoordinatesInField(const std::pair<int, int>& coords, int length, Direction direction) const;
        [[nodiscard]] bool shipsAreContacting(const std::pair<int, int>& coords) const;
        [[nodiscard]] bool intersectionShips(const std::pair<int, int>& coordinates, int length, Direction direction) const;
    public:
        GameField(int width, int height);
        GameField(const GameField& other);
        GameField(GameField&& other) noexcept;
        GameField& operator=(const GameField& other);
        GameField& operator=(GameField&& other) noexcept;
        GameField(
            int width,
            int height, ShipsField  shipsCoordinateMap,
                  ShipCoordinatesSet  attacksOnField
        );
        [[nodiscard]] int getHeight() const;
        [[nodiscard]] int getWidth() const;
        [[nodiscard]] const ShipsField& getShipsCoordinateMap() const;
        [[nodiscard]] const ShipCoordinatesSet& getAttacksOnField () const;
        [[nodiscard]] bool isAllShipsDestroyed() const;
        [[nodiscard]] bool canPlaceShip(const std::pair<int, int>& initialCoordinate, Direction direction, int length) const;

        void updateShipsCoordinateMap(const ShipsField& newMap);
        void placeShip(Ship* ship, const std::pair<int, int>& initialCoordinate, Direction direction);
        bool intersectsWithArea(const std::pair<int, int>& center, int radius);
        std::pair<int, int> removeShip(const std::pair<int, int>& coordinate);

        AttackResult attack(const std::pair<int, int>& initialCoordinate, int damageCount);
        void clear();
    };
} // namespace cpp_warships::application