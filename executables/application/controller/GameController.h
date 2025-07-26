#pragma once

#include "../model/game/GameStateDTO.h"
#include "game-states/GameState.h"
#include "game-states/MenuGameState.h"
#include <cpp_warships/input_reader/include/InputReader.h>
#include <cpp_warships/input_reader/include/console_reader/ConsoleInputReader.h>

template <typename T>
struct is_reader_derivative {
private:
    template <class U>
    static std::true_type test(const cpp_warships::input_reader::InputReader<U> *);
    static std::false_type test(...);

public:
    static constexpr bool value = decltype(test(std::declval<T *>()))::value;
};

template <typename T>
concept InputReaderDerivative = is_reader_derivative<T>::value;

template <InputReaderDerivative T>
class GameController {
private:
    GameState *currentState;
    GameStateDTO *currentMatchData;
    StateContext stateContext;
    cpp_warships::input_reader::InputReader<> *inputReader;

public:
    explicit GameController(T *inputReader = new cpp_warships::input_reader::console_reader::ConsoleInputReader())
        : currentState(nullptr),
          currentMatchData(nullptr),
          stateContext(StateContext(inputReader)),
          inputReader(inputReader) {}

    ~GameController() {
        delete currentState;
        delete currentMatchData;
        delete inputReader;
    }

    void run() {
        currentState = new MenuGameState(stateContext);
        currentState->openState();
        currentState->updateState();

        while (!stateContext.currentMatchData ||
               (stateContext.currentMatchData && !stateContext.currentMatchData->isFinished)) {
            GameState *newState = currentState->transitToState();

            if (newState) {
                currentState->closeState();
                currentState = newState;
                currentState->openState();
            }

            currentState->updateState();
        }
    }
};
