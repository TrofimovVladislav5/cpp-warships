#pragma once
#include <stdexcept>
#include <string>

class BattleException : public std::exception {
private:
    std::string message;

public:
    explicit BattleException(const std::string& msg);
    void displayError() const;
};
