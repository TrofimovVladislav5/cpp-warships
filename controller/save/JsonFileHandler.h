#pragma once
#include <fstream>
#include "../cmake-build-debug/_deps/nlohmann_json-src/single_include/nlohmann/json.hpp"

class JsonFileHandler {
public:
    enum class Mode {Read, Write};
    JsonFileHandler(const std::string& filename, Mode mode);
    ~JsonFileHandler();
    [[nodiscard]] std::ifstream& getInputStream();
    [[nodiscard]] std::ofstream& getOutputStream();
    JsonFileHandler& operator<<(const nlohmann::json& j);
    JsonFileHandler& operator>>(nlohmann::json& j);

private:
    Mode mode;
    std::ifstream inputFile;
    std::ofstream outputFile;
};

