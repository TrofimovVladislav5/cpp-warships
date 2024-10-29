#include "ShipPlacementException.h"
#include "ViewHelper.h"


ShipPlacementException::ShipPlacementException(const std::string& msg)
    :   message(msg)
{}

void ShipPlacementException::displayError() const {
    ViewHelper::errorOut("Ship placement Error: " + message);
}
