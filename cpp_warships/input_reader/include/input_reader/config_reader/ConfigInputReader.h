#pragma once

#include <input_reader/InputReader.h>

#include <string>
#include <vector>

namespace cpp_warships::input_reader::config_reader {
    class ConfigInputReader : public InputReader<> {
       private:
        std::vector<std::string> fileContents;
        InputReader* shadowReader;
        size_t linesExecuted;

       public:
        explicit ConfigInputReader(const std::string& filename);
        std::string readCommand() override;
    };
}  // namespace cpp_warships::input_reader::config_reader
