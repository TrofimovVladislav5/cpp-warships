#include "GameFieldView.h"

#include <algorithm>
#include <iostream>

#include "FieldCellView.h"

namespace cpp_warships::application {

    GameFieldView::GameFieldView(GameField* gameField) : gameField(gameField) {}

    GameFieldView::~GameFieldView() {}

    bool isPresent(const std::unordered_map<std::pair<int, int>, int, hashFunc>& shipCoordinates,
                   const std::pair<int, int>& scanCell) {
        return shipCoordinates.find(scanCell) != shipCoordinates.end();
    }

    bool isShipNear(std::unordered_map<std::pair<int, int>, int, hashFunc>& shipCoordinates,
                    std::pair<int, int> scanCell) {
        return isPresent(shipCoordinates, std::make_pair(scanCell.first, scanCell.second + 1)) ||
               isPresent(shipCoordinates, std::make_pair(scanCell.first, scanCell.second - 1)) ||
               isPresent(shipCoordinates, std::make_pair(scanCell.first - 1, scanCell.second)) ||
               isPresent(shipCoordinates, std::make_pair(scanCell.first + 1, scanCell.second)) ||
               isPresent(shipCoordinates, std::make_pair(scanCell.first - 1, scanCell.second - 1)) ||
               isPresent(shipCoordinates, std::make_pair(scanCell.first + 1, scanCell.second + 1)) ||
               isPresent(shipCoordinates, std::make_pair(scanCell.first + 1, scanCell.second - 1)) ||
               isPresent(shipCoordinates, std::make_pair(scanCell.first - 1, scanCell.second + 1));
    }

    void GameFieldView::printUpperBar(std::pair<int, int> boundaries) {
        std::vector<FieldCellView> upperBarContents;
        upperBarContents.emplace_back(" ", true, false, false, true);

        const std::string abc = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        for (int x = std::min(boundaries.first, boundaries.second);
             x < std::max(boundaries.first, boundaries.second); x++) {
            std::string content(1, abc[x]);
            upperBarContents.emplace_back(content, true, x == boundaries.second - 1, false, false);
        }

        printFieldLine(upperBarContents, false);
        upperBarContents.clear();
    }

    void GameFieldView::printFieldLine(std::vector<FieldCellView>& cells, bool isLast) {
        for (int i = 0; i < cells.size(); i++) {
            std::cout << cells[i].getTopEdge(true, i == cells.size() - 1);
        }

        std::cout << std::endl;
        for (int i = 0; i < cells.size(); i++) {
            std::cout << cells[i].getMiddleLayer(true, i == cells.size() - 1);
        }

        std::cout << std::endl;
        if (isLast) {
            for (int i = 0; i < cells.size(); i++) {
                std::cout << cells[i].getBottomEdge(true, i == cells.size() - 1);
            }
            std::cout << std::endl;
        }
    }

    void GameFieldView::printField(
            bool isOpponentView, bool shipHints,
            std::unordered_map<std::pair<int, int>, int, hashFunc> shipCoordinates,
            std::unordered_map<std::pair<int, int>, int, hashFunc> destroyedShipCoordinates,
            std::unordered_set<std::pair<int, int>, hashFunc> attacksOnField) {
        for (int y = 0; y < gameField->getHeight(); y++) {
            std::vector<FieldCellView> result;
            result.emplace_back(std::to_string(y + 1), false, true, y == gameField->getHeight() - 1,
                                true);

            for (int x = 0; x < gameField->getWidth(); x++) {
                std::pair<int, int> coord = std::make_pair(x, y);

                std::string coordContent;
                if (!isOpponentView) {
                    bool isShip = isPresent(shipCoordinates, coord);
                    bool isNear = !isShip && isShipNear(shipCoordinates, coord);
                    bool isAttack = attacksOnField.find(coord) != attacksOnField.end();

                    if (isShip)
                        coordContent = std::to_string(shipCoordinates.at(coord));
                    else if (isAttack)
                        coordContent = "X";
                    else if (isNear && shipHints)
                        coordContent = "X";
                    else
                        coordContent = " ";
                } else {
                    bool isAttack = attacksOnField.find(coord) != attacksOnField.end();
                    bool isShip = isPresent(shipCoordinates, coord);
                    bool isDamagedShip = isShip && isAttack;
                    bool isDestroyed = isPresent(destroyedShipCoordinates, coord);
                    bool isNearDestroyed = isShipNear(destroyedShipCoordinates, coord);

                    if (isDestroyed)
                        coordContent = "0";
                    else if (isNearDestroyed)
                        coordContent = "X";
                    else if (isDamagedShip && isAttack)
                        coordContent = std::to_string(shipCoordinates.at(coord));
                    else if (isAttack)
                        coordContent = "X";
                    else
                        coordContent = " ";
                }

                result.emplace_back(coordContent, false, x == gameField->getWidth() - 1,
                                    y == gameField->getHeight() - 1, false);
            }

            printFieldLine(result, y == gameField->getHeight() - 1);
        }
    }

    void GameFieldView::displayField(bool isOpponentView = false, bool shipHints) {
        std::unordered_map<std::pair<int, int>, int, hashFunc> shipCoordinates;
        std::unordered_map<std::pair<int, int>, int, hashFunc> destroyedShipCoordinates;
        std::unordered_set<std::pair<int, int>, hashFunc> attacksOnField =
                gameField->getAttacksOnField();

        for (const auto& [ship, coordinates] : gameField->getShipsCoordinateMap()) {
            int index = 0;
            for (const auto& coordinate : coordinates) {
                shipCoordinates[coordinate] = ship->getSegmentHitPoints(index);
                if (ship->isDestroyed()) {
                    destroyedShipCoordinates[coordinate] = ship->getSegmentHitPoints(index);
                }

                index++;
            }
        }

        this->printUpperBar(std::make_pair(0, gameField->getHeight()));

        printField(isOpponentView, shipHints, shipCoordinates, destroyedShipCoordinates,
                   attacksOnField);
    }

    void GameFieldView::displayScan(std::pair<int, int> leftUpper) {
        this->printUpperBar(std::make_pair(std::min(leftUpper.first, gameField->getWidth()),
                                           std::min(leftUpper.first + 2, gameField->getWidth())));

        std::unordered_map<std::pair<int, int>, std::string, hashFunc> shipCoordinates;
        for (const auto& [ship, coordinates] : gameField->getShipsCoordinateMap()) {
            int index = 0;
            for (const auto& coordinate : coordinates) {
                shipCoordinates[coordinate] = std::to_string(ship->getSegmentHitPoints(index));
                index++;
            }
        }

        for (int y = leftUpper.second; y < std::min(leftUpper.second + 2, gameField->getHeight());
             y++) {
            std::string result = std::to_string(y) + " ";

            for (int x = leftUpper.first; x < std::min(leftUpper.first + 2, gameField->getWidth());
                 x++) {
                int boundedWidth = std::clamp(x, 0, gameField->getWidth() - 1);
                int boundedHeight = std::clamp(y, 0, gameField->getHeight() - 1);
                std::pair<int, int> coord = std::make_pair(boundedWidth, boundedHeight);

                if (shipCoordinates.find(coord) != shipCoordinates.end()) {
                    result += shipCoordinates.at(coord) + " ";
                } else {
                    result += "+ ";
                }
            }
            std::cout << result << std::endl;
        }
    }
} // namespace cpp_warships::application