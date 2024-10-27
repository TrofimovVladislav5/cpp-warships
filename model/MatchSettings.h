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
    explicit MatchSettings(int fieldSize = 10);
    ~MatchSettings();
    GameField* getPlayerField();
    GameField* getOpponentField();
    ShipManager* getPlayerManager();
    ShipManager* getOpponentManager();
    void setPlayerManager(const  std::vector<int>& shipsLengths);
    void setFieldSize(int size);
    void setShipsSize(std::vector<int>& sizes);
};