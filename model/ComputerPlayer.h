#pragma once
#include "AttackHandler.h"
#include "ComputerPlayer.h"
#include "defaults//FieldCoordinateHelper.h"
#include "game/GameField.h"

using FieldCoordinate = std::pair<int, int>;

class ComputerPlayer {
private:
    GameField* field;
    std::vector<FieldCoordinate> emptyCells;
    std::vector<FieldCoordinate> currentShotCells;
    [[nodiscard]] FieldCoordinate getRandomAttackCoordinate();
    [[nodiscard]] FieldCoordinate getRandomNeighbourCoordinate(FieldCoordinate coordinate);
    [[nodiscard]] FieldCoordinate getRandomDirectionCoordinate();
    bool proceedShot(FieldCoordinate coordinate);

public:
    explicit ComputerPlayer(GameField* field);
    ~ComputerPlayer();
    bool makeAShot();
};
