#include "JsonFileHandler.h"

JsonFileHandler::JsonFileHandler(const std::string& filename, Mode mode)
    : mode(mode)
{
    if (mode == Mode::Read) {
        inputFile.open(filename, std::ios::in);
        if (!inputFile.is_open()) {
            throw std::runtime_error("Could not open file for reading: " + filename);
        }
    } else if (mode == Mode::Write) {
        outputFile.open(filename, std::ios::out);
        if (!outputFile.is_open()) {
            throw std::runtime_error("Could not open file for writing: " + filename);
        }
    }
}

JsonFileHandler::~JsonFileHandler() {
    if (inputFile.is_open()) {
        inputFile.close();
    }
    if (outputFile.is_open()) {
        outputFile.close();
    }
}

std::ifstream& JsonFileHandler::getInputStream() {
    if (mode != Mode::Read) {
        throw std::runtime_error("File is not open for reading");
    }
    return inputFile;
}

std::ofstream& JsonFileHandler::getOutputStream() {
    if (mode != Mode::Write) {
        throw std::runtime_error("File is not open for writing");
    }
    return outputFile;
}

JsonFileHandler& JsonFileHandler::operator<<(const nlohmann::json& j) {
    if (mode != Mode::Write) {
        throw std::runtime_error("File is not open for writing");
    }
    outputFile << j.dump(4);
    return *this;
}

JsonFileHandler& JsonFileHandler::operator>>(nlohmann::json& j) {
    if (mode != Mode::Read) {
        throw std::runtime_error("File is not open for reading");
    }
    inputFile >> j;
    return *this;
}