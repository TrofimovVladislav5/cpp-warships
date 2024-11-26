#pragma once
#include "GameFieldView.h"
#include "PlaceShipController.h"

class PlaceShipControllerView {
private:
    PlaceShipController* controller;
    GameFieldView* currentFieldView;
public:
    explicit PlaceShipControllerView(PlaceShipController* controller);
    void displayShipsLeft() const;
    void displayCurrentField() const;
};
