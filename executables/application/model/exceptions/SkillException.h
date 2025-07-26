#pragma once

#include <string>

namespace cpp_warships::application {

    class SkillException : public std::exception {
    private:
        std::string message;

    public:
        explicit SkillException(const std::string& msg);
        void displayError() const;
    };
} // namespace cpp_warships::application