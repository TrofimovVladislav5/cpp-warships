#pragma once
#include "ShipManager.h"
#include "GameField.h"

class MatchSettings {
private:
    GameField* playerField;
    GameField* opponentField;
    ShipManager* playerShipManager;
    ShipManager* opponentShipManager;
    int fieldSize;
public:
    MatchSettings(int fieldSize = 10);
    ~MatchSettings();
    GameField* getPlayerField();
    GameField* getOpponentField();
    ShipManager* getPlayerManager();
    ShipManager* getOpponentShipManager();
    void setFieldSize(int size);
    int getFieldSize();
};