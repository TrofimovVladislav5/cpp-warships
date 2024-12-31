#include "StateContext.h"

StateContext::StateContext(InputReader<>* inputReader)
    : currentMatchData(nullptr)
    , initialGameSubState(nullptr)
    , inputReader(inputReader)
{}

InputReader<> * StateContext::getInputReader() const {
    return inputReader;
}
