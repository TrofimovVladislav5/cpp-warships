#pragma once

#include <string>
#include <vector>

#include "../InputReader.h"

namespace cpp_warships::input_reader::config_reader {

    class ConfigInputReader : public InputReader<> {
    private:
        std::vector<std::string> fileContents;
        InputReader<>* shadowReader;
        int linesExecuted;
    public:
        explicit ConfigInputReader(const std::string &filename);
        std::string readCommand() override;
    };
} // namespace cpp_warships::input_reader::config_reader
