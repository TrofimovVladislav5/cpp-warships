#pragma once

#include <fstream>
#include <nlohmann/json.hpp>

namespace cpp_warships::application {

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
} // namespace cpp_warships::application
