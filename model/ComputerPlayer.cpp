#include "ComputerPlayer.h"
#include "GameField.h"
#include "defaults//FieldCoordinateHelper.h"
#include "Structures.h"
#include <utility>
#include <algorithm>
#include <random>
#include <stdexcept>

ComputerPlayer::ComputerPlayer(GameField* field)
    : field(field)
{
    this->emptyCells = std::vector<FieldCoordinate>();
    this->currentShotCells = std::vector<FieldCoordinate>();
}

ComputerPlayer::~ComputerPlayer() {
    this->emptyCells.clear();
    this->currentShotCells.clear();
}

FieldCoordinate ComputerPlayer::getRandomAttackCoordinate() {
    FieldCoordinate result;
    do {
        std::random_device random_device;
        std::mt19937 mersenneEngine{random_device()};
        std::uniform_int_distribution<> distX{0, field->getWidth() - 1};
        std::uniform_int_distribution<> distY{0, field->getHeight() - 1};
        result = {distX(mersenneEngine), distY(mersenneEngine)};
    } while (std::find(emptyCells.begin(), emptyCells.end(), result) != emptyCells.end());

    return result;
}

FieldCoordinate ComputerPlayer::getRandomNeighbourCoordinate(FieldCoordinate coordinate) {
    std::vector<FieldCoordinate> potentialNeighbours = {
        {coordinate.first - 1, coordinate.second},
        {coordinate.first + 1, coordinate.second},
        {coordinate.first, coordinate.second - 1},
        {coordinate.first, coordinate.second + 1}
    };

    std::vector<FieldCoordinate> finalNeighbours = {};
    for (const auto& neighbour : potentialNeighbours) {
        // TODO: replace with field.size - 1
        bool isInsideField = neighbour.first >= 0 && neighbour.first < field->getWidth() && neighbour.second >= 0 && neighbour.second < field->getHeight();
        bool isShot = std::find(emptyCells.begin(), emptyCells.end(), neighbour) != emptyCells.end();
        if (isInsideField && !isShot) {
            finalNeighbours.emplace_back(neighbour);
        }
    }

    std::shuffle(finalNeighbours.begin(), finalNeighbours.end(), std::mt19937(std::random_device()()));
    if (finalNeighbours.size() == 0) {
        throw std::out_of_range("No empty neighbours, but the ship is not completely destroyed. \n\t Unreachable state, unluck.");
    }
    return finalNeighbours[0];
}

FieldCoordinate ComputerPlayer::getRandomDirectionCoordinate() {
    std::random_device random_device;
    std::mt19937 mersenneEngine{random_device()};
    std::uniform_int_distribution<> forward{0, 1};
    bool forwardValue = forward(mersenneEngine);

    if (currentShotCells[0].first == currentShotCells[1].first) {
        int maxY = FieldCoordinateHelper::findMaxPairValue(currentShotCells, false);
        int minY = FieldCoordinateHelper::findMinPairValue(currentShotCells, false);
        // TODO: replace with field.size - 1
        if ((forwardValue && maxY < field->getHeight() - 1) || (!forwardValue && minY == 0 && maxY < field->getHeight() - 1)) {
            return {currentShotCells[0].first, maxY + 1};
        // TODO: replace with field.size - 1
        }
        if (minY > 0) {
            return {currentShotCells[0].first, minY - 1};
        }
    } else {
        int maxX = FieldCoordinateHelper::findMaxPairValue(currentShotCells, true);
        int minX = FieldCoordinateHelper::findMinPairValue(currentShotCells, true);
        // TODO: replace with field.size - 1
        if ((forwardValue && maxX < field->getWidth() - 1) || (!forwardValue && minX == 0 && maxX < field->getWidth() - 1)) {
            return {maxX + 1, currentShotCells[0].second};
        }
        // TODO: replace with field.size - 1
        if (minX > 0) {
            return {minX - 1, currentShotCells[0].second};
        }
    }

    throw std::out_of_range("It seems that the destroy wasn't handled \n\t Unreachable state, unluck.");
}

bool ComputerPlayer::proceedShot(FieldCoordinate coordinate) {
    AttackResult isHit = field->attack(coordinate,2);
    if (isHit == AttackResult::damaged) {
        currentShotCells.push_back(coordinate);
    }
    else if (isHit == AttackResult::destroyed) {
        currentShotCells.clear();
    }
    this->emptyCells.push_back(coordinate);

    return (isHit != AttackResult::miss);
}

bool ComputerPlayer::makeAShot() {
    FieldCoordinate attackCoordinate;
    if (currentShotCells.empty()) {
        attackCoordinate = this->getRandomAttackCoordinate();
    } else if (currentShotCells.size() == 1) {
        attackCoordinate = this->getRandomNeighbourCoordinate(currentShotCells[0]);
    } else {
        attackCoordinate = this->getRandomDirectionCoordinate();
    }
    return proceedShot(attackCoordinate);
}

