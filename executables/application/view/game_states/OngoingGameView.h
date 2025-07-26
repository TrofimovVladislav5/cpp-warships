#pragma once

#include "GameView.h"

namespace cpp_warships::application {

    class OngoingGameView : public GameView {
    public:
        virtual void displayAvailableCommands(const GameStateDTO* currentMatchData);
        virtual void displayOpenState();
        virtual void displayCloseState();
    };
} // namespace cpp_warships::application