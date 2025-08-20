#pragma once

#include <stdexcept>
#include <string>

namespace cpp_warships::application {

    class BattleException : public std::exception {
    private:
        std::string message;

    public:
        explicit BattleException(const std::string& msg);
        void displayError() const;
    };
} // namespace cpp_warships::application