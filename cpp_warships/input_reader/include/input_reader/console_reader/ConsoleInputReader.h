#pragma once

#include <input_reader/InputReader.h>

#include <string>

namespace cpp_warships::input_reader::console_reader {
    class ConsoleInputReader : public InputReader<> {
    public:
        std::string readCommand() override;
    };
}  // namespace cpp_warships::input_reader::console_reader
