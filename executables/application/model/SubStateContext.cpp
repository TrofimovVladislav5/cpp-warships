#include "SubStateContext.h"

namespace cpp_warships::application {
    SubStateContext::SubStateContext(GameStateDTO *matchData, input_reader::InputReader<> *inputReader)
        : inputReader(inputReader), matchDTO(matchData) {}

    input_reader::InputReader<> *SubStateContext::getInputReader() const { return inputReader; }
} // namespace cpp_warships::application
