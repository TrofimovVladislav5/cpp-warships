#pragma once

#include <cpp_warships/input_parser/include/model/Parser.h>

#include "OngoingGameSubState.h"
#include "../MatchBuilder.h"
#include "../save/GameSaveCreator.h"
#include "../../view/GamePauseView.h"


class PauseOngoingGameSubState final : public OngoingGameSubState {
private:
    cpp_warships::input_parser::SchemeMap<void> inputScheme;
    MatchBuilder matchBuilder;
    GamePauseView view;
    OngoingGameSubState* loadedSubState;
    void handleResume(cpp_warships::input_parser::ParsedOptions options);
    void handleSave(cpp_warships::input_parser::ParsedOptions options);
    void handleLoad(cpp_warships::input_parser::ParsedOptions options);
public:
    explicit PauseOngoingGameSubState(SubStateContext* context);
    void openSubState() override;
    void closeSubState() override;
    void updateSubState() override;
    OngoingGameSubState* transitToSubState() override;
};
