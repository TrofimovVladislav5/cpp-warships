#pragma once
#include <map>
#include <vector>

#include "game/Ship.h"

class ShipManager {
private:
    std::vector<Ship*> ships;
    std::map<int, int> shipsSize;
public:
    explicit ShipManager(const std::map<int, int> &shipsSize);
    ~ShipManager();
    std::vector<Ship*> getShips();
    Ship* operator[](int index);
    void addShip(int size);
    void removeShipNumber(int indexRemoving);
    void clear();
};
