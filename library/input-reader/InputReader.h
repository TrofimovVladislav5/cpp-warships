#pragma once

template <typename T>
class InputReader {
public:
    virtual ~InputReader() = default;

    virtual T readCommand() = 0;
};