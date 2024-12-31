#pragma once
#include "../controller/ShipManager.h"
#include <iostream>
class ShipManagerView {
private:
    ShipManager& manager;
public:
    ShipManagerView(ShipManager& manager);
    void displayShips();
};