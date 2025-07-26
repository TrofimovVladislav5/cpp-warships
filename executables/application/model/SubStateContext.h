#pragma once
#include "game/GameStateDTO.h"
#include <cpp_warships/input_reader/include/InputReader.h>

class SubStateContext {
private:
    cpp_warships::input_reader::InputReader<>* inputReader;

public:
    explicit SubStateContext(GameStateDTO* matchData, cpp_warships::input_reader::InputReader<>* inputReader = nullptr);
    GameStateDTO* matchDTO;
    [[nodiscard]] cpp_warships::input_reader::InputReader<>* getInputReader() const;
};
