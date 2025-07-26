#pragma once

#include <cpp_warships/input_reader/include/InputReader.h>

#include "game/GameStateDTO.h"

namespace cpp_warships::application {

    class SubStateContext {
    private:
        input_reader::InputReader<>* inputReader;

    public:
        explicit SubStateContext(GameStateDTO* matchData, input_reader::InputReader<>* inputReader = nullptr);
        GameStateDTO* matchDTO;
        [[nodiscard]] input_reader::InputReader<>* getInputReader() const;
    };
} // namespace cpp_warships::application