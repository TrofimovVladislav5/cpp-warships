#pragma once
#include <iostream>

#include "../controller/ShipManager.h"
class ShipManagerView {
private:
    ShipManager& manager;

public:
    ShipManagerView(ShipManager& manager);
    void displayShips();
};