#include "../../../include/state_machine/states/StateContext.h"

namespace cpp_warships::application {

    StateContext::StateContext(input_reader::InputReader<>* inputReader)
        : inputReader(inputReader)
        , currentMatchData(nullptr)
        , initialGameSubState(nullptr) {}

    input_reader::InputReader<>* StateContext::getInputReader() const { return inputReader; }
} // namespace cpp_warships::application