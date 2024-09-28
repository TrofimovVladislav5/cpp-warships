cmake_minimum_required(VERSION 3.29)
project(cpp_warships)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(controller)

add_executable(cpp_warships
        controller/GameController.cpp
        controller/GameController.h
        model/GameField.cpp
        model/Player.cpp
        model/ShipManager.cpp
        controller/game-states/GameState.cpp
        controller/game-states/GameState.h
        view/GameView.cpp
        view/GameView.h
        controller/game-states/MenuGameState.cpp
        controller/game-states/MenuGameState.h
        controller/game-states/OngoingGameState.cpp
        controller/game-states/OngoingGameState.h
        main.cpp
        __temp__/WarShips.cpp
        controller/ongoing-game-substates/PauseOngoingGameSubstate.h
        controller/ongoing-game-substates/PauseOngoingGameSubstate.cpp
        controller/ongoing-game-substates/BattleOngoingGameSubstate.cpp
        controller/ongoing-game-substates/BattleOngoingGameSubstate.h
        controller/ongoing-game-substates/FinishOngoingGameSubstate.cpp
        controller/ongoing-game-substates/FinishOngoingGameSubstate.h
        controller/ongoing-game-substates/InitiateOngoingGameSubstate.cpp
        controller/ongoing-game-substates/InitiateOngoingGameSubstate.h
        model/GameField.h
        model/Ship.cpp
        model/Ship.h
        model/Structures.cpp
        model/Structures.h
        model/GameStateDTO.cpp
        model/GameStateDTO.h
        model/StateContext.cpp
        model/StateContext.h
        view/GameMenuView.cpp
        view/GameMenuView.h
        view/ViewHelper.cpp
        view/ViewHelper.h
        controller/game-states/ShutdownGameState.cpp
        controller/game-states/ShutdownGameState.h
        view/OngoingGameView.cpp
        view/OngoingGameView.h
)
