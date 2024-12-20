#pragma once
#include <string>

#include "input-reader/InputReader.h"

class ConsoleInputReader : public InputReader<> {
public:
    std::string readCommand() override;
};
