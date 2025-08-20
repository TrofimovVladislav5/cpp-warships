#include "../../../include/state_machine/states/ShutdownGameState.h"

#include <cpp_warships/utilities/include/ViewHelper.h>

namespace cpp_warships::application {

    ShutdownGameState::ShutdownGameState(StateContext& context)
        : GameState(context)
    {}

    void ShutdownGameState::openState() {
        context.currentMatchData->isFinished = true;
    }

    void ShutdownGameState::updateState() {
        ViewHelper::consoleOut("Good good bye!");
    }

    void ShutdownGameState::closeState() {

    }

    GameState* ShutdownGameState::transitToState() {
        return nullptr;
    }
} // namespace cpp_warships::application
