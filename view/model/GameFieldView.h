#pragma once
#include "../model/GameField.h"

class GameFieldView {
private:
    GameField* gameField;
    void printUpperBar();
public:
    GameFieldView(GameField* gameField);
    ~GameFieldView();
    void displayField();
};