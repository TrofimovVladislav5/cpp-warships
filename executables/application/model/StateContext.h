#pragma once

#include <cpp_warships/input_reader/include/InputReader.h>

#include "game/GameStateDTO.h"
#include "../controller/ongoing_game_substates/OngoingGameSubState.h"

namespace cpp_warships::application {

    class StateContext {
        input_reader::InputReader<>* inputReader;

    public:
        explicit StateContext(input_reader::InputReader<>* inputReader = nullptr);
        ~StateContext() = default;

        GameStateDTO* currentMatchData;
        OngoingGameSubState* initialGameSubState;
        [[nodiscard]] input_reader::InputReader<>* getInputReader() const;
    };
} // namespace cpp_warships::application