#pragma once
#include "Ship.h"
#include <vector>
#include <iostream>

class ShipManager {
private:
    std::vector<Ship*> ships;
public:
    explicit ShipManager(const std::vector<int>& shipsSize);
    ~ShipManager();
    std::vector<Ship*>& getShips();
    Ship* operator[](int index);
    void addShip(int size);
    void removeShipNumber(int indexRemoving);
};