#pragma once

#include <cpp_warships/input_parser/include/model/Parser.h>

#include "OngoingGameSubState.h"
#include "../MatchBuilder.h"
#include "../../GameSaveCreator.h"
#include "../../GamePauseView.h"

namespace cpp_warships::application {

    class PauseOngoingGameSubState final : public OngoingGameSubState {
    private:
        input_parser::SchemeMap<void> inputScheme;
        MatchBuilder matchBuilder;
        GamePauseView view;
        OngoingGameSubState* loadedSubState;
        void handleResume(input_parser::ParsedOptions options);
        void handleSave(input_parser::ParsedOptions options);
        void handleLoad(input_parser::ParsedOptions options);
    public:
        explicit PauseOngoingGameSubState(SubStateContext* context);
        void openSubState() override;
        void closeSubState() override;
        void updateSubState() override;
        OngoingGameSubState* transitToSubState() override;
    };
} // namespace cpp_warships::application