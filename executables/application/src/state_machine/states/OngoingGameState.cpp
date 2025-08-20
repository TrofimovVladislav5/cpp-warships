#include "../../../include/state_machine/states/OngoingGameState.h"

#include <iostream>
#include <cpp_warships/utilities/include/ViewHelper.h>
#include <cpp_warships/utilities/include/StateMessages.h>

#include "../../../include/state_machine/GameState.h"
#include "../../../include/state_machine/states/TransitGameState.h"
#include "../../../include/state_machine/substates/FinishOngoingGameSubState.h"

namespace cpp_warships::application {

    OngoingGameState::OngoingGameState(StateContext& context)
        : GameState(context)
        , currentSubState(nullptr)
    {}

    void OngoingGameState::openState() {
        StateMessages::displayGreetingMessage("Match");
        currentSubState = context.initialGameSubState;
    }

    void OngoingGameState::updateState() {
        OngoingGameSubState* newSubState = currentSubState->transitToSubState();
        if (newSubState) {
            currentSubState->closeSubState();
            currentSubState = newSubState;
            currentSubState->openSubState();
        }

        currentSubState->updateSubState();
    }

    void OngoingGameState::closeState() {
        ViewHelper::consoleOut("Leaving the game");
    }

    GameState* OngoingGameState::transitToState() {
        if (typeid(*currentSubState).name() == typeid(FinishOngoingGameSubState).name()) {
            return new TransitGameState(context);
        }

        return nullptr;
    }
} // namespace cpp_warships::application
