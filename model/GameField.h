#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "Ship.h"
#include "Structures.h"

class GameField {
private:
    int width;
    int height;
    std::vector<std::vector<Cell>> field;
    std::unordered_map<Ship*, std::unordered_set<std::pair<int, int>,hashFunc>> shipsCoordinateMap;
    std::unordered_set<std::pair<int, int>, hashFunc> attackCoordinateMap;
    bool shipCoordinatesInField(std::pair<int, int> coords, int length, Direction direction) const;
    bool shipsAreContacting(std::pair<int, int> coords) const;
    bool intersectionShips(std::pair<int , int> coordinates, int length, Direction direction) const;
public:
    GameField(int width, int height);
    bool placeShip(Ship* ship, std::pair<int, int> initialCoordinate, Direction direction);
    void showField();
    //     for (int y = 0; y < height;y++) {
    //         for (int x = 0; x < width;x++) {
    //             field[y][x] = Cell::unknown;
    //         }
    //     }
    //
    //     for (const auto& [ship, coordinates] : shipsCoordinateMap) {
    //         int index = 0;
    //         for (const auto& coord : coordinates) {
    //             SegmentState segmentState = ship->getSegment(index);
    //             if (segmentState == SegmentState::intact) {
    //                 field[coord.second][coord.first] = Cell::ship_int;
    //             } else if (segmentState == SegmentState::damaged) {
    //                 field[coord.second][coord.first] = Cell::ship_damaged;
    //             } else if (segmentState == SegmentState::destroyed) {
    //                 field[coord.second][coord.first] = Cell::ship_destroyed;
    //             }
    //             index++;
    //         }
    //     }
    //
    //     scanCellsNextToDestroyedShip(field, shipsCoordinateMap);
    //
    //     std::string result;
    //     std::string upperBar = "№ ";
    //     for (int x = 0; x < width; x++) {
    //         upperBar += std::to_string(x) + " ";
    //     }
    //     std::cout << std::endl;
    //     std::cout << upperBar << std::endl;
    //     for (int y = 0; y < height;y++) {
    //         result.clear();
    //         result += std::to_string(y) + " ";
    //         for (int x = 0; x < width;x++) {
    //             switch (field[y][x]){
    //                 case Cell::empty:
    //                     result += "+ ";
    //                     break;
    //                 case Cell::ship_int:
    //                     result += "2 ";
    //                     break;
    //                 case Cell::ship_damaged:
    //                     result += "1 ";
    //                     break;
    //                 case Cell::ship_destroyed:
    //                     result += "0 ";
    //                     break;
    //                 case Cell::unknown:
    //                     result += "* ";
    //                     break;
    //             }
    //         }
    //         std::cout << result << std::endl;
    //     }
    // }

    bool attack(std::pair<int, int> initialCoordinate, int damageCount);
    bool validateCoordinates(std::pair<int, int> coordToCheck);
    void scanCellsNextToDestroyedShip(std::vector<std::vector<Cell>>& field, std::unordered_map<Ship*,
                                      std::unordered_set<std::pair<int, int>,hashFunc>>& shipsCoordinateMap);
};
