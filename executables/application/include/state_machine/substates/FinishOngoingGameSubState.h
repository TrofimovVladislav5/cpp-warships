#pragma once

#include <cpp_warships/input_parser/include/model/Parser.h>

#include "OngoingGameSubState.h"

namespace cpp_warships::application {

    class FinishOngoingGameSubState : public OngoingGameSubState {
    public:
        explicit FinishOngoingGameSubState(SubStateContext* context);
        void openSubState() override;
        void updateSubState() override;
        void closeSubState() override;
        OngoingGameSubState* transitToSubState() override;
    };
} // namespace cpp_warships::application