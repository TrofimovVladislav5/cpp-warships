#pragma once

#include "states/StateContext.h"

namespace cpp_warships::application {
    class GameState {
    protected:
        StateContext& context;

    public:
        explicit GameState(StateContext& context);
        virtual ~GameState() = default;
        virtual void openState() = 0;
        virtual void updateState() = 0;
        virtual void closeState() = 0;
        virtual GameState* transitToState() = 0;
    };
} // namespace cpp_warships::application
