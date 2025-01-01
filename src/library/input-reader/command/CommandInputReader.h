#pragma once
#include <map>
#include <string>

#include "input-reader/InputReader.h"

class CommandInputReader : public InputReader<> {
private:
    std::map<std::string, std::string> keymap;
public:
    explicit CommandInputReader(const std::string &filename, char delimiter = ';');
    std::string readCommand() override;
};
