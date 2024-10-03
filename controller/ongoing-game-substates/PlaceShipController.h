#pragma once
#include "model/StateContext.h"

class PlaceShipController {
private:
    int minimalCountShips;
    StateContext& context;
public:
    int currentCountShips;
    PlaceShipController(StateContext& context);
    ~PlaceShipController();
    void addShip();
    void deleteShip();
};