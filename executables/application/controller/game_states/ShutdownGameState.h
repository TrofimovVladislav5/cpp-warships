#pragma once

#include "GameState.h"

namespace cpp_warships::application {

    class ShutdownGameState : public GameState {
    public:
        ShutdownGameState(StateContext& context);
        ~ShutdownGameState() = default;

        void openState() override;
        void updateState() override;
        void closeState() override;
        GameState* transitToState() override;
    };
} // namespace cpp_warships::application
