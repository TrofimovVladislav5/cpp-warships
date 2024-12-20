#pragma once
#include "game-states/GameState.h"
#include "../model/game/GameStateDTO.h"
#include "game-states/MenuGameState.h"
#include "input-reader/InputReader.h"
#include "input-reader/console/ConsoleInputReader.h"
#include <type_traits>

template <typename T>
struct is_reader_derivative {
private:
    template <class U>
    static std::true_type test(const InputReader<U>*);
    static std::false_type test(...);
public:
    static constexpr bool value = decltype(test(std::declval<T*>()))::value;
};

template <typename T>
concept InputReaderDerivative = is_reader_derivative<T>::value;


template <InputReaderDerivative T>
class GameController {
private:
    GameState *currentState;
    GameStateDTO *currentMatchData;
    StateContext stateContext;
    InputReader<>* inputReader;
public:
    explicit GameController(T* inputReader = new ConsoleInputReader())
        : currentState(nullptr)
        , currentMatchData(nullptr)
        , stateContext(StateContext(inputReader))
        , inputReader(inputReader)
    {}

    ~GameController() {
        delete currentState;
        delete currentMatchData;
        delete inputReader;
    }

    void run() {
        currentState = new MenuGameState(stateContext);
        currentState->openState();
        currentState->updateState();

        while (
            !stateContext.currentMatchData ||
            (stateContext.currentMatchData && !stateContext.currentMatchData->isFinished)
        ) {
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
