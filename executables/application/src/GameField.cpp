#include "../include/GameField.h"

#include <ranges>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "../include/Structures.h"

namespace cpp_warships::application {
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

    GameField::GameField(GameField&& other) noexcept
        : width(other.getWidth())
        , height(other.getHeight())
        , shipsCoordinateMap(other.getShipsCoordinateMap())
        , attacksOnField(other.getAttacksOnField())
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

    GameField& GameField::operator=(GameField&& other) noexcept {
        if (this != &other) {
            this->width = other.getWidth();
            this->height = other.getHeight();
            this->shipsCoordinateMap = other.getShipsCoordinateMap();
            this->attacksOnField = other.getAttacksOnField();
        }

        return *this;
    }

    GameField::GameField(
        int width,
        int height,
        ShipsField shipsCoordinateMap,
        ShipCoordinatesSet attacksOnField
    )
        : width(width)
        , height(height)
        , shipsCoordinateMap(std::move(shipsCoordinateMap))
        , attacksOnField(std::move(attacksOnField))
    {}

    int GameField::getHeight() const {
        return height;
    }

    int GameField::getWidth() const {
        return width;
    }

    const ShipsField& GameField::getShipsCoordinateMap() const {
        return this->shipsCoordinateMap;
    }

    const ShipCoordinatesSet& GameField::getAttacksOnField() const {
        return this->attacksOnField;
    }

    bool GameField::canPlaceShip(
        const std::pair<int, int>& initialCoordinate,
        Direction direction,
        int length
    ) const {
        if (!shipCoordinatesInField(initialCoordinate, length, direction) || intersectionShips(initialCoordinate, length, direction)) {
            return false;
        }
        return true;
    }

    void GameField::placeShip(
        Ship* ship,
        const std::pair<int, int>& initialCoordinate,
        Direction direction
    ) {
        int length = ship->getLength();
        for (int i = 0; i < length;i++) {
            std::pair<int, int> newCoordinate = initialCoordinate;
            if (direction == Direction::horizontal) newCoordinate.first += i;
            else newCoordinate.second += i;

            this->shipsCoordinateMap[ship].insert(newCoordinate);
        }
    }

    bool GameField::intersectsWithArea(const std::pair<int, int>& center, const int radius) {
        for (int dy = -radius; dy <= radius; ++dy) {
            for (int dx = -radius; dx <= radius; ++dx) {
                int newX = center.first + dx;
                int newY = center.second + dy;
                if (newX >= 0 && newX < width && newY >= 0 && newY < height) {
                    std::pair checkCoord = {newX, newY};
                    for (const auto& coordinates : shipsCoordinateMap | std::views::values) {
                        if (coordinates.find(checkCoord) != coordinates.end()) {
                            return true;
                        }
                    }
                }
            }
        }

        return false;
    }

    bool GameField::shipCoordinatesInField(
        const std::pair<int, int>& coords,
        const int length,
        const Direction direction
    ) const {
        if (direction == Direction::horizontal) {
            return coords.first + length <= width;
        }
        return coords.second + length <= height;
    }

    bool GameField::shipsAreContacting(const std::pair<int, int>& coords) const {
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

    bool GameField::intersectionShips(
        const std::pair<int, int>& coordinates,
        const int length,
        const Direction direction
    ) const {
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


    AttackResult GameField::attack(
        const std::pair<int, int>& initialCoordinate,
        const int damageCount
    ) {
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
        for (const auto& ship : shipsCoordinateMap | std::views::keys) {
            if (!ship->isDestroyed()) {
                return false;
            }
        }
        return true;
    }

    void GameField::updateShipsCoordinateMap(const ShipsField& newMap) {
        shipsCoordinateMap.clear();
        shipsCoordinateMap = newMap;
    }
} // namespace cpp_warships::application