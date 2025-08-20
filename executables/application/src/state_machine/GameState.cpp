#include "../../include/state_machine/GameState.h"

#include "../../include/state_machine/states/StateContext.h"

namespace cpp_warships::application {
    GameState::GameState(StateContext& context) : context(context) {}
} // namespace cpp_warships::application
