#pragma once
#include "../model/GameField.h"

class GameFieldView {
private:
    GameField* gameField;
    void printUpperBar(std::pair<int, int> boundaries);
public:
    GameFieldView(GameField* gameField);
    ~GameFieldView();
    void displayField(bool isOpponentView);
    void displayScan(std::pair<int, int> leftUpper);
};