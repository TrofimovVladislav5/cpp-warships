#pragma once
#include "Ship.h"
class ShipManager{
private:
    std::vector<Ship> ships;
public:
    void addShip(Ship* ship);
    void printStateShips();
};