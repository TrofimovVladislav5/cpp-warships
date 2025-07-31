#pragma once

#include <iostream>

#include "ShipManager.h"

namespace cpp_warships::application {

    class ShipManagerView {
    private:
        ShipManager& manager;

    public:
        ShipManagerView(ShipManager& manager);
        void displayShips();
    };
} // namespace cpp_warships::application