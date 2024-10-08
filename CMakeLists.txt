cmake_minimum_required(VERSION 3.22.1)
project(cpp_warships)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(controller)

add_executable(cpp_warships
#        controller/GameController.cpp
#        controller/GameController.h
#        model/GameField.cpp
#        model/Player.cpp
#        model/ShipManager.cpp
#        controller/game-states/GameState.cpp
#        controller/game-states/GameState.h
#        view/GameView.cpp
#        view/GameView.h
#        controller/game-states/MenuGameState.cpp
#        controller/game-states/MenuGameState.h
#        controller/game-states/OngoingGameState.cpp
#        controller/game-states/OngoingGameState.h
#        controller/ongoing-game-substates/PauseOngoingGameSubstate.h
#        controller/ongoing-game-substates/PauseOngoingGameSubstate.cpp
#        controller/ongoing-game-substates/BattleOngoingGameSubstate.cpp
#        controller/ongoing-game-substates/BattleOngoingGameSubstate.h
#        controller/ongoing-game-substates/FinishOngoingGameSubstate.cpp
#        controller/ongoing-game-substates/FinishOngoingGameSubstate.h
#        controller/ongoing-game-substates/InitiateOngoingGameSubstate.cpp
#        controller/ongoing-game-substates/InitiateOngoingGameSubstate.h
#        controller/ongoing-game-substates/PlaceShipController.cpp
#        model/GameField.h
#        model/Ship.cpp
#        model/Ship.h
#        model/Structures.cpp
#        model/Structures.h
#        model/GameStateDTO.cpp
#        model/GameStateDTO.h
#        model/StateContext.cpp
#        model/StateContext.h
#        model/MatchSettings.cpp
#        view/GameMenuView.cpp
#        view/GameMenuView.h
#        controller/game-states/ShutdownGameState.cpp
#        controller/game-states/ShutdownGameState.h
#        view/OngoingGameView.cpp
#        view/OngoingGameView.h
        main.cpp
        view/ViewHelper.cpp
        view/ViewHelper.h
        library/parser/ParserParameter.cpp
        library/parser/ParserParameter.h
        library/parser/ParserCommandInfo.cpp
        library/parser/ParserCommandInfo.h
        library/parser/Parser.cpp
        library/parser/Parser.h
        library/StringHelper.cpp
        library/StringHelper.h
        library/TypesHelper.cpp
        library/TypesHelper.h
        library/parser-builder/ParserCommandBuilder.cpp
        library/parser-builder/ParserCommandBuilder.h
        library/parser-builder/ConfigCommandBuilder.cpp
        library/parser-builder/ConfigCommandBuilder.h
        library/parser-builder/ParserBuilderDirector.cpp
        library/parser-builder/ParserBuilderDirector.h
        library/parser-builder/ParserParameterBuilder.cpp
        library/parser-builder/ParserParameterBuilder.h
        library/parser-builder/DefaultParameterBuilder.cpp
        library/parser-builder/DefaultParameterBuilder.h
)
