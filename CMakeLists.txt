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
        model/Ship.cpp
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
        __temp__/WarShips.cpp)
