#pragma once

#include <stdexcept>
#include <string>

namespace cpp_warships::application {

    class ShipPlacementException : std::exception {
    private:
        std::string message;

    public:
        explicit ShipPlacementException(const std::string& msg);
        void displayError() const;
    };
} // namespace cpp_warships::application