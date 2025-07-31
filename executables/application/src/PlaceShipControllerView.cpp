#include "../include/PlaceShipControllerView.h"

#include <iostream>
#include <cpp_warships/utilities/include/ViewHelper.h>

namespace cpp_warships::application {

    PlaceShipControllerView::PlaceShipControllerView(PlaceShipController* controller)
        : controller(controller), currentFieldView(new GameFieldView(controller->getCurrentField())) {}

    void PlaceShipControllerView::displayShipsLeft() const {
        std::cout << "All available ships (length : count)" << std::endl;

        int emptyTypes = 0;
        for (const auto& pair : controller->getAvailableLengthShips()) {
            if (pair.second != 0) {
                std::cout << "Ship of length " << pair.first << " is needed to place " << pair.second
                          << " times" << std::endl;
            } else {
                emptyTypes++;
            }
        }

        if (emptyTypes == controller->getAvailableLengthShips().size()) {
            ViewHelper::consoleOut("All ships are placed, storage is empty");
        }
    }

    void PlaceShipControllerView::displayCurrentField() const {
        currentFieldView->displayField(false);
    }
} // namespace cpp_warships::application