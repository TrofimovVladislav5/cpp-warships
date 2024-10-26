#include "../model/GameField.h"
#include "../model/Ship.h"
#include "model/GameFieldView.h"
#include <iostream>

GameFieldView::GameFieldView(GameField* gameField)
    : gameField(gameField)
{}

GameFieldView::~GameFieldView() {
    delete gameField;
}
bool isPresent(const std::unordered_map<std::pair<int, int>, int, hashFunc>& shipCoordinates, const std::pair<int, int>& scanCell) {
    return shipCoordinates.find(scanCell) != shipCoordinates.end();
}

bool isShipNear(std::unordered_map<std::pair<int, int>, int,hashFunc>& shipCoordinates, std::pair<int, int> scanCell) {
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
    std::string upperBar = "№ ";
    for (int x = std::min(boundaries.first, boundaries.second); x < std::max(boundaries.first, boundaries.second); x++) {
        upperBar += std::to_string(x) + " ";
    }
    std::cout << upperBar << std::endl;
}

void GameFieldView::displayField(bool isOpponentView = false) {
    std::unordered_map<std::pair<int, int>, int, hashFunc> shipCoordinates;
    std::unordered_map<std::pair<int, int>, int, hashFunc> destroyedShipCoordinates;
    std::unordered_set<std::pair<int, int>, hashFunc> attacksOnField = gameField->getAttacksOnField();

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
    for (int y = 0; y < gameField->getHeight(); y++) {
        std::string result;
        result += std::to_string(y) + " ";
        for (int x = 0; x < gameField->getWidth(); x++) {
            std::pair<int, int> coord = std::make_pair(x, y);

            if (isPresent(shipCoordinates, coord)) {
                if (isOpponentView && attacksOnField.find(coord) == attacksOnField.end() && shipCoordinates.at(coord) != 0) {
                    result += "* ";
                } else {
                    result += std::to_string(shipCoordinates.at(coord)) + " ";
                }
            } else if (isShipNear(destroyedShipCoordinates, coord)) {
                result += "- ";
            } else {
                result += "* ";
            }
        }
        std::cout << result << std::endl;
    }
}


void GameFieldView::displayScan(std::pair<int, int> leftUpper) {
    this->printUpperBar(std::make_pair(leftUpper.first, leftUpper.first + 2));
    std::unordered_map<std::pair<int, int>, std::string, hashFunc> shipCoordinates;

    for (const auto& [ship, coordinates] : gameField->getShipsCoordinateMap()) {
        int index = 0;
        for (const auto& coordinate : coordinates) {
            shipCoordinates[coordinate] = std::to_string(ship->getSegmentHitPoints(index));
            index++;
        }
    }

    for (int y = leftUpper.second; y < leftUpper.second + 2; y++) {
        std::string result;
        result += std::to_string(y) + " ";
        for (int x = leftUpper.first; x < leftUpper.first + 2; x++) {
            std::pair<int, int> coord = std::make_pair(x, y);

            if (shipCoordinates.find(coord) != shipCoordinates.end()) {
                result += shipCoordinates.at(coord) + " ";
            } else {
                result += "+ ";
            }
        }
        std::cout << result << std::endl;
    }
}