#pragma once

#include "GameFieldView.h"
#include "PlaceShipController.h"

namespace cpp_warships::application {

    class PlaceShipControllerView {
    private:
        PlaceShipController* controller;
        GameFieldView* currentFieldView;

    public:
        explicit PlaceShipControllerView(PlaceShipController* controller);
        void displayShipsLeft() const;
        void displayCurrentField() const;
    };
} // namespace cpp_warships::application