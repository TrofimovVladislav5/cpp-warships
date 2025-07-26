#pragma once

#include <cpp_warships/input_reader/include/InputReader.h>

#include "game/GameStateDTO.h"
#include "../controller/ongoing-game-substates/OngoingGameSubState.h"

class StateContext {
    cpp_warships::input_reader::InputReader<>* inputReader;

public:
    explicit StateContext(cpp_warships::input_reader::InputReader<>* inputReader = nullptr);
    ~StateContext() = default;

    GameStateDTO* currentMatchData;
    OngoingGameSubState* initialGameSubState;
    [[nodiscard]] cpp_warships::input_reader::InputReader<>* getInputReader() const;
};
