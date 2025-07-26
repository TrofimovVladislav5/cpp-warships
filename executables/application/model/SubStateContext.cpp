#include "SubStateContext.h"

SubStateContext::SubStateContext(GameStateDTO *matchData, cpp_warships::input_reader::InputReader<> *inputReader)
    : inputReader(inputReader), matchDTO(matchData) {}

cpp_warships::input_reader::InputReader<> *SubStateContext::getInputReader() const { return inputReader; }
