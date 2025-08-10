#include "../../include/save_system/JsonFileHandler.h"

namespace cpp_warships::application {

    JsonFileHandler::JsonFileHandler(const std::string& filename, Mode mode)
        : mode(mode)
    {
        switch (mode) {
            case Mode::Read:
                handleReadFile(filename);
                break;
            case Mode::Write:
                handleWriteFile(filename);
                break;
            default:
                throw std::logic_error("Unknown mode: " + std::to_string(static_cast<int>(mode)));
        }
    }

    JsonFileHandler::~JsonFileHandler() {
        if (inputFile.is_open()) {
            inputFile.close();
        } else if (outputFile.is_open()) {
            outputFile.close();
        }
    }

    void JsonFileHandler::handleReadFile(const std::string& filename) {
        inputFile.open(filename, std::ios::in);
        if (!inputFile.is_open()) {
            throw std::runtime_error("Could not open file for reading: " + filename);
        }
    }

    void JsonFileHandler::handleWriteFile(const std::string& filename) {
        outputFile.open(filename, std::ios::out);
        if (!outputFile.is_open()) {
            throw std::runtime_error("Could not open file for writing: " + filename);
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

    JsonFileHandler& JsonFileHandler::operator<<(const nlohmann::json& jsonObject) {
        if (mode != Mode::Write) {
            throw std::runtime_error("File is not open for writing");
        }

        outputFile << jsonObject.dump(4);
        return *this;
    }

    JsonFileHandler& JsonFileHandler::operator>>(nlohmann::json& jsonObject) {
        if (mode != Mode::Read) {
            throw std::runtime_error("File is not open for reading");
        }

        inputFile >> jsonObject;
        return *this;
    }
} // namespace cpp_warships::application