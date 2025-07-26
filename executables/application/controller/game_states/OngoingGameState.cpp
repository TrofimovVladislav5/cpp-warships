#include "OngoingGameState.h"

#include <iostream>
#include <cpp_warships/utilities/include/ViewHelper.h>
#include <cpp_warships/utilities/include/StateMessages.h>

#include "GameState.h"
#include "TransitGameState.h"
#include "../../controller/ongoing_game_substates/FinishOngoingGameSubState.h"

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
