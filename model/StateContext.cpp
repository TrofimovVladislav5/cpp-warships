#include "StateContext.h"

StateContext::StateContext() {
    currentMatch = nullptr;
    finishCallback = nullptr;
    finishMatchCallback = nullptr;
}
