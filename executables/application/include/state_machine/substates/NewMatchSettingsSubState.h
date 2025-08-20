#pragma once

#include "OngoingGameSubState.h"
#include "../../MatchSettingsController.h"

namespace cpp_warships::application {

    class NewMatchSettingsSubState : public OngoingGameSubState {
    private:
        input_parser::SchemeMap<void> inputScheme;
        MatchSettingsController* controller;
        MatchSettings* currentSettings;
        void handleMatchSettings(input_parser::ParsedOptions options);

    public:
        explicit NewMatchSettingsSubState(SubStateContext* context);
        ~NewMatchSettingsSubState() override;
        void openSubState() override;
        void closeSubState() override;
        void updateSubState() override;
        OngoingGameSubState* transitToSubState() override;
    };
} // namespace cpp_warships::application
