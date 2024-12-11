#include "StateContext.h"

StateContext::StateContext() {
    loadFileName = "";
    currentMatch = nullptr;
    finishCallback = nullptr;
    finishMatchCallback = nullptr;
}
