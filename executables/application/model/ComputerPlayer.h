#pragma once

#include <cpp_warships/utilities/include/FieldCoordinateHelper.h>

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
    std::vector<FieldCoordinate> getShotCellsNeighbours();
    bool proceedShot(FieldCoordinate coordinate);

public:
    explicit ComputerPlayer(GameField* field);
    ~ComputerPlayer();
    bool makeAShot();
    bool isWin() const;
};
