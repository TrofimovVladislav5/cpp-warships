#pragma once

#include "../game_states/OngoingGameView.h"

namespace cpp_warships::application {

    class InitiateOngoingGameView : public OngoingGameView {
    public:
        void displayAvailableCommands(const GameStateDTO* currentMatchData) override;
        void displayOpenState() override;
        void displayCloseState() override;
    };
} // namespace cpp_warships::application