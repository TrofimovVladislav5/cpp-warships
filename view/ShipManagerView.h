#pragma once
#include "ShipManager.h"
#include <iostream>
class ShipManagerView {
private:
    ShipManager& manager;
public:
    ShipManagerView(ShipManager& manager);
    void displayShips();
};