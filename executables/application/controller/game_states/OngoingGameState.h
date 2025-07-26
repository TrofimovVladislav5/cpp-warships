#pragma once

#include "GameState.h"
#include "../ongoing_game_substates/OngoingGameSubState.h"
#include "../../model/StateContext.h"

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
