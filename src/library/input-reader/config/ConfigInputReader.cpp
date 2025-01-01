#include "ConfigInputReader.h"
#include <filesystem>
#include <fstream>
#include <iostream>

#include "input-reader/console/ConsoleInputReader.h"

ConfigInputReader::ConfigInputReader(const std::string &filename)
    : linesExecuted(0)
    , fileContents({})
    , shadowReader(new ConsoleInputReader())
{
    std::ifstream file(filename);

    std::vector<std::string> fileContents;
    if (!file.is_open() || std::filesystem::is_directory(filename)) {
        std::cerr << "Error opening file!" << std::endl;
    } else {
        std::string line;
        while (std::getline(file, line)) {
            fileContents.push_back(line);
        }

        file.close();
    }

    this->fileContents = fileContents;
}

std::string ConfigInputReader::readCommand() {
    auto command = linesExecuted >= this->fileContents.size()
        ? shadowReader->readCommand()
        : this->fileContents[linesExecuted++];

    linesExecuted = std::min(linesExecuted, static_cast<int>(this->fileContents.size()));

    return command;
}