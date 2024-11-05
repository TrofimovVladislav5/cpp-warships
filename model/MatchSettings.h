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
    int damageCount;
    bool isActiveDoubleDamage;
    std::pair<int, int> scanCoordinate;
public:
    explicit MatchSettings(int fieldSize = 10);
    ~MatchSettings();
    GameField* getPlayerField();
    GameField* getOpponentField();
    ShipManager* getPlayerManager();
    ShipManager* getOpponentManager();
    int getDamage() const;
    bool getIsActiveDoubleDamage() const;
    const std::pair<int, int>& coordinateToScan();
    void setActiveDoubleDamage(bool isActive);
    void setPlayerManager(const  std::vector<int>& shipsLengths);
    void setFieldSize(int size);
    void setDamageCount(int damage);
    void setScanCoordinates(const std::pair<int, int> &scanCoordinate);
};
