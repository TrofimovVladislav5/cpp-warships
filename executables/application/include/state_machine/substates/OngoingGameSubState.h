#pragma once

#include "SubStateContext.h"

namespace cpp_warships::application {

    class OngoingGameSubState {
    protected:
        SubStateContext* context;
    public:
        explicit OngoingGameSubState(SubStateContext* context);
        virtual ~OngoingGameSubState() = default;
        virtual void openSubState() = 0;
        virtual void updateSubState() = 0;
        virtual void closeSubState() = 0;
        virtual OngoingGameSubState* transitToSubState() = 0;
    };
} // namespace cpp_warships::application