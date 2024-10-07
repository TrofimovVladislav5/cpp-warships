#pragma once
#include "Ship.h"
class ShipManager {
private:
    std::vector<Ship*> ships;
public:
    ShipManager(const std::vector<int>& shipsSize);
    ~ShipManager();
    Ship* operator[](int index);
    void addShip(int size);
    void removeShipNumber(int indexRemoving);
    void showShipsInfo();
};