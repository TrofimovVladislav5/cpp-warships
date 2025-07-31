#pragma once

#include "StateContext.h"
#include "../GameState.h"
#include "../substates/OngoingGameSubState.h"

namespace cpp_warships::application {

    class OngoingGameState : public GameState {
    protected:
        OngoingGameSubState* currentSubState;

    public:
        OngoingGameState(StateContext &context);
        void openState() override;
        void updateState() override;
        void closeState() override;
        GameState* transitToState() override;
    };
} // namespace cpp_warships::application
