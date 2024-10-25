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

void GameFieldView::printUpperBar() {
    std::string upperBar = "N ";
    for (int x = 0; x < gameField->getWidth(); x++) {
        upperBar += std::to_string(x) + " ";
    }

    std::cout << std::endl;
    std::cout << upperBar << std::endl;
}

void GameFieldView::displayField() {
    std::unordered_map<std::pair<int, int>, int, hashFunc> shipCoordinates;
    std::unordered_map<std::pair<int, int>, int, hashFunc> destroyedShipCoordinates;

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

    this->printUpperBar();
    for (int y = 0; y < gameField->getHeight(); y++) {
        std::string result;
        result += std::to_string(y) + " ";
        for (int x = 0; x < gameField->getWidth(); x++) {
            if (isPresent(shipCoordinates, std::make_pair(x, y))) {
                result += std::to_string(shipCoordinates.at(std::make_pair(x, y))) + " ";
            } else if (isShipNear(destroyedShipCoordinates, std::make_pair(x, y))) {
                result += "- ";
            } else {
                result += "* ";
            }
        }
        std::cout << result << std::endl;
    }
}