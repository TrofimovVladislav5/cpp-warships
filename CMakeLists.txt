cmake_minimum_required(VERSION 3.22.1)
project(cpp_warships)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)
include_directories(controller)
include_directories(library)
include_directories(library/parser)
include_directories(library/parser-builder)
include_directories(library/defaults)
include_directories(model)
include_directories(view)
include_directories(view/game-states)
include_directories(view/model)


include(FetchContent)

FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.10.0
)

FetchContent_MakeAvailable(nlohmann_json)

add_executable(cpp_warships
        main.cpp
        library/ViewHelper.cpp
        library/ViewHelper.h
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
        library/parser-builder/CommandBuildDirector.cpp
        library/parser-builder/CommandBuildDirector.h
        library/parser-builder/ParameterBuildDirector.cpp
        library/parser-builder/ParameterBuildDirector.h
        library/parser/__showcase__/parserShowcase.h
        library/parser/__showcase__/parserShowcaseInline.h
        library/defaults/DefaultHelp.cpp
        library/defaults/DefaultHelp.h
        library/defaults/DefaultParserError.cpp
        library/defaults/DefaultParserError.h
        controller/game-states/MenuGameState.cpp
        controller/game-states/MenuGameState.h
        controller/game-states/GameState.cpp
        controller/game-states/GameState.h
        controller/game-states/OngoingGameState.cpp
        controller/game-states/OngoingGameState.h
        controller/game-states/ShutdownGameState.cpp
        controller/game-states/ShutdownGameState.h
        controller/BattleController.cpp
        controller/BattleController.h
        controller/ongoing-game-substates/BattleOngoingGameSubstate.cpp
        controller/ongoing-game-substates/BattleOngoingGameSubstate.h
        controller/ongoing-game-substates/FinishOngoingGameSubstate.cpp
        controller/ongoing-game-substates/FinishOngoingGameSubstate.h
        controller/ongoing-game-substates/InitiateOngoingGameSubstate.cpp
        controller/ongoing-game-substates/InitiateOngoingGameSubstate.h
        controller/ongoing-game-substates/NewMatchSettingsSubState.cpp
        controller/ongoing-game-substates/NewMatchSettingsSubState.h
        controller/ongoing-game-substates/PauseOngoingGameSubstate.cpp
        controller/ongoing-game-substates/PauseOngoingGameSubstate.h
        controller/PlaceShipController.cpp
        controller/PlaceShipController.h
        controller/MatchSettingsController.cpp
        controller/MatchSettingsController.h
        controller/GameController.cpp
        controller/GameController.h
        model/game/GameField.cpp
        model/game/GameField.h
        model/game/GameStateDTO.cpp
        model/game/GameStateDTO.h
        model/game/MatchSettings.cpp
        model/game/MatchSettings.h
        model/Player.cpp
        model/Player.h
        model/game/Segment.cpp
        model/game/Segment.h
        model/game/Ship.cpp
        model/game/Ship.h
        model/ShipManager.cpp
        model/ShipManager.h
        model/StateContext.cpp
        model/StateContext.h
        model/Structures.cpp
        model/Structures.h
        view/game-states/GameView.cpp
        view/game-states/GameView.h
        view/game-states/OngoingGameView.cpp
        view/game-states/OngoingGameView.h
        view/game-states/ShutdownGameView.cpp
        view/game-states/ShutdownGameView.h
        view/game-substates/InitiateOngoingGameView.cpp
        view/game-substates/InitiateOngoingGameView.h
        #        view/game-substates/BattleOngoingGameSubState.cpp
#        view/game-substates/BattleOngoingGameSubState.h
#        view/game-substates/FinishOngoingGameSubState.cpp
#        view/game-substates/FinishOngoingGameSubState.h
        view/GameFieldView.cpp
        view/GameFieldView.h
        library/defaults/Initials.h
        model/skills/DoubleDamage.h
        model/skills/DoubleDamage.cpp
        model/skills/Scanner.cpp
        model/skills/Scanner.h
        model/skills/ShootingRandomlySkill.cpp
        model/skills/ShootingRandomlySkill.h
        model/skills/SkillManager.h
        model/skills/SkillManager.cpp
        model/skills/SkillFactory.h
        model/skills/Skill.h
        model/skills/Skill.cpp
        model/exceptions/ShipPlacementException.h
        model/exceptions/ShipPlacementException.cpp
        model/exceptions/SkillException.h
        model/exceptions/SkillException.cpp
        model/exceptions/BattleException.h
        model/exceptions/BattleException.cpp
        model/ComputerPlayer.h
        model/ComputerPlayer.cpp
        library/defaults/FieldCoordinateHelper.h
        library/defaults/FieldCoordinateHelper.cpp
        library/defaults/StateMessages.cpp
        library/defaults/StateMessages.h
        controller/ongoing-game-substates/OngoingGameSubState.cpp
        controller/ongoing-game-substates/OngoingGameSubState.h
        model/SubStateContext.cpp
        model/SubStateContext.h
        controller/ongoing-game-substates/NewMatchSettingsSubState.h
        view/FieldCellView.cpp
        view/FieldCellView.h
        view/PlaceShipControllerView.cpp
        view/PlaceShipControllerView.h
        model/PlayerFieldState.cpp
        model/PlayerFieldState.h
        controller/AttackHandler.cpp
        controller/AttackHandler.h
        view/SkillManagerView.cpp
        view/SkillManagerView.h
        model/save/Serializer.h
        model/save/GameSaveCreator.h
        model/save/GameSaveCreator.cpp
        model/save/SerializerManager.cpp
        model/save/SerializerManager.h
        model/save/SerializerGameField.h
        model/save/SerializerGameField.cpp
        model/save/Serializer.cpp
        model/save/SerializerMatchSettings.h
        model/save/SerializerMatchSettings.cpp
        model/save/SerializerSkillManager.h
        model/save/SerializerSimplifyObjects.h
        model/save/SerializerSimplifyObjects.cpp
        model/save/SerializerSkillManager.cpp
        model/save/SerializerFactory.h
        model/save/JsonFileHandler.h
        model/save/JsonFileHandler.cpp
        view/ShipManagerView.cpp
        #        model/save/test.cpp
)

target_link_libraries(cpp_warships PRIVATE nlohmann_json::nlohmann_json)