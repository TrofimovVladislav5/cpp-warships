#include "../../../include/state_machine/substates/FinishOngoingGameSubState.h"

#include <cpp_warships/utilities/include/StateMessages.h>
#include <cpp_warships/utilities/include/TypesHelper.h>
#include <cpp_warships/utilities/include/ViewHelper.h>

namespace cpp_warships::application {

    FinishOngoingGameSubState::FinishOngoingGameSubState(SubStateContext* context)
        : OngoingGameSubState(context)
    {}

    void FinishOngoingGameSubState::openSubState() {
        std::string welcomeMessage = std::string("You have finished the game with the score of ")
            .append(std::to_string(context->matchDTO->roundNumber))
            .append(" rounds completed");

        ViewHelper::consoleOut(welcomeMessage, 1);
    }

    void FinishOngoingGameSubState::updateSubState() {
    }

    void FinishOngoingGameSubState::closeSubState() {
    }

    OngoingGameSubState* FinishOngoingGameSubState::transitToSubState() {
        return nullptr;
    }
} // namespace cpp_warships::application
