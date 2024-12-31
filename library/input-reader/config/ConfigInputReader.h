#pragma once
#include <string>
#include <vector>

#include "input-reader/InputReader.h"

class ConfigInputReader : public InputReader<> {
private:
    std::vector<std::string> fileContents;
    InputReader<>* shadowReader;
    int linesExecuted;
public:
    explicit ConfigInputReader(const std::string &filename);
    std::string readCommand() override;
};
