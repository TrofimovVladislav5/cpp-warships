#pragma once
#include <stdexcept>

class SkillException : public std::exception{
private:
    std::string message;
public:
    explicit SkillException(const std::string& msg);
    void displayError() const;
};



