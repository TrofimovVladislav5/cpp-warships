#pragma once
#include <map>
#include <vector>

#include "../model/game/Ship.h"

class ShipManager {
private:
    std::vector<Ship*> ships;
    std::map<int, int> shipsSizes;
public:
    explicit ShipManager(const std::map<int, int> &shipsSize);
    explicit ShipManager(const std::map<int, int> &shipsSize, std::vector<Ship*> ships);
    ~ShipManager();
    std::vector<Ship*> getShips() const;
    std::map<int, int> getShipsSizes() const;
    Ship* operator[](int index);
    void addShip(int size);
    void removeShipNumber(int indexRemoving);
    void clear();
};
