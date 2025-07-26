#pragma once

#include "../../model/game/GameStateDTO.h"

namespace cpp_warships::application {

    class GameView {
    public:
        virtual ~GameView() = default;
        virtual void displayAvailableCommands(const GameStateDTO* currentMatchData) = 0;
        virtual void displayOpenState() = 0;
        virtual void displayCloseState() = 0;
    };
} // namespace cpp_warships::application