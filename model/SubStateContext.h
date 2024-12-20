#pragma once
#include "game/GameStateDTO.h"
#include "input-reader/InputReader.h"

class SubStateContext {
private:
    InputReader<>* inputReader;
public:
    explicit SubStateContext(GameStateDTO* matchData, InputReader<>* inputReader = nullptr);
    GameStateDTO* matchDTO;
    [[nodiscard]] InputReader<>* getInputReader() const;
};
