#pragma once
#include "ComputerPlayer.h"
#include "GameField.h"
#include "defaults//FieldCoordinateHelper.h"

using FieldCoordinate = std::pair<int, int>;

class ComputerPlayer {
private:
    GameField* field;
    std::vector<FieldCoordinate> emptyCells;
    std::vector<FieldCoordinate> currentShotCells;
    FieldCoordinate getRandomAttackCoordinate();
    FieldCoordinate getRandomNeighbourCoordinate(FieldCoordinate coordinate);
    FieldCoordinate getRandomDirectionCoordinate();
    bool proceedShot(FieldCoordinate coordinate);

public:
    explicit ComputerPlayer(GameField* field);
    ~ComputerPlayer();
    bool makeAShot();
};
