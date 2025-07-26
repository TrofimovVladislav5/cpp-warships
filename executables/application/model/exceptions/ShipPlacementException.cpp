#include "ShipPlacementException.h"

#include "ViewHelper.h"

namespace cpp_warships::application {

    ShipPlacementException::ShipPlacementException(const std::string& msg) : message(msg) {}

    void ShipPlacementException::displayError() const {
        ViewHelper::errorOut("Ship placement Error: " + message);
    }
} // namespace cpp_warships::application