#include "StateContext.h"

namespace cpp_warships::application {

    StateContext::StateContext(input_reader::InputReader<>* inputReader)
        : currentMatchData(nullptr), initialGameSubState(nullptr), inputReader(inputReader) {}

    input_reader::InputReader<>* StateContext::getInputReader() const { return inputReader; }
} // namespace cpp_warships::application