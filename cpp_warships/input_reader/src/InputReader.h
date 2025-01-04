#pragma once
#include <string>


template <typename T = std::string>
class InputReader {
public:
    virtual ~InputReader() = default;

    virtual T readCommand() = 0;
};