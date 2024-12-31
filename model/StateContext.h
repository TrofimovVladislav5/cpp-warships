#pragma once

#include "game/GameStateDTO.h"
#include "input-reader/InputReader.h"
#include "ongoing-game-substates/OngoingGameSubState.h"

class StateContext {
    InputReader<>* inputReader;

public:
    explicit StateContext(InputReader<>* inputReader = nullptr);
    ~StateContext() = default;

    GameStateDTO* currentMatchData;
    OngoingGameSubState* initialGameSubState;
    [[nodiscard]] InputReader<>* getInputReader() const;
};
