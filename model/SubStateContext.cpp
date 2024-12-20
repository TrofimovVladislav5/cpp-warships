#include "SubStateContext.h"

SubStateContext::SubStateContext(GameStateDTO *matchData, InputReader<> *inputReader)
    : inputReader(inputReader)
    , matchDTO(matchData)
{}

InputReader<> * SubStateContext::getInputReader() const {
    return inputReader;
}
