#include <iostream>
#include "GameView.h"
#include "model/GameField.h"
void GameView::displayGameField(GameField* field){
    field->printGameField();
}
