#pragma once
#include "FieldCellView.h"
#include "../model/game/GameField.h"

class GameFieldView {
private:
    GameField* gameField;
    void printUpperBar(std::pair<int, int> boundaries);
    void printFieldLine(std::vector<FieldCellView>& cells, bool isLast = false);
public:
    GameFieldView(GameField* gameField);
    ~GameFieldView();
    void GameFieldView::printField(bool isOpponentView, bool shipHints, std::unordered_map<std::pair<int, int>, int, hashFunc> shipCoordinates, std::unordered_map<std::pair<int, int>, int, hashFunc> destroyedShipCoordinates, std::unordered_set<std::pair<int, int>, hashFunc> attacksOnField);
    void displayField(bool isOpponentView, bool shipHints = true);
    void displayScan(std::pair<int, int> leftUpper);
};
