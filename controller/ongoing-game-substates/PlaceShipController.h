#pragma once
#include "model/StateContext.h"

class PlaceShipController {
private:
    int minimalCountShips;
    int minimalFieldSize;
public:
    PlaceShipController(StateContext& context);
    ~PlaceShipController();
    void placementShips();
    void deleteShip();
};