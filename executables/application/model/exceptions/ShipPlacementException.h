#pragma once
#include <stdexcept>
#include <string>

class ShipPlacementException : std::exception {
private:
    std::string message;

public:
    explicit ShipPlacementException(const std::string& msg);
    void displayError() const;
};
