#include "StateContext.h"

StateContext::StateContext(cpp_warships::input_reader::InputReader<>* inputReader)
    : currentMatchData(nullptr), initialGameSubState(nullptr), inputReader(inputReader) {}

cpp_warships::input_reader::InputReader<>* StateContext::getInputReader() const { return inputReader; }
