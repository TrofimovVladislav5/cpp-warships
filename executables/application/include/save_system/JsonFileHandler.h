#pragma once

#include <nlohmann/json.hpp>
#include <fstream>
#include <string>

namespace cpp_warships::application {

    class JsonFileHandler {
    public:
        enum class Mode { Read, Write };

        JsonFileHandler(const std::string& filename, Mode mode);
        ~JsonFileHandler();

        [[nodiscard]] std::ifstream& getInputStream();
        [[nodiscard]] std::ofstream& getOutputStream();

        JsonFileHandler& operator<<(const nlohmann::json& j);
        JsonFileHandler& operator>>(nlohmann::json& j);

    private:
        void handleReadFile(const std::string& filename);
        void handleWriteFile(const std::string& filename);
        Mode mode;
        std::ifstream inputFile;
        std::ofstream outputFile;
    };

} // namespace cpp_warships::application