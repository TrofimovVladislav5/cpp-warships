# Compile files
add_library(warships_head
        # Shared vocabulary
        include/application/head/Color.h
        include/application/head/Theme.h src/application/head/Theme.cpp
        include/application/head/Queries.h
        # What the session currently is
        include/application/head/session/Application.h src/application/head/session/Application.cpp
        include/application/head/session/BattleJournal.h src/application/head/session/BattleJournal.cpp
        # What a screen may ask for
        include/application/head/intents/Intent.h
        include/application/head/intents/IntentContext.h
        include/application/head/intents/ShowScreenIntent.h src/application/head/intents/ShowScreenIntent.cpp
        include/application/head/intents/StartMatchIntent.h src/application/head/intents/StartMatchIntent.cpp
        include/application/head/intents/ChangeThemeIntent.h src/application/head/intents/ChangeThemeIntent.cpp
        include/application/head/intents/PlaceShipIntent.h src/application/head/intents/PlaceShipIntent.cpp
        include/application/head/intents/RemoveShipIntent.h src/application/head/intents/RemoveShipIntent.cpp
        include/application/head/intents/ShuffleFleetIntent.h src/application/head/intents/ShuffleFleetIntent.cpp
        include/application/head/intents/BeginBattleIntent.h src/application/head/intents/BeginBattleIntent.cpp
        include/application/head/intents/FireAtIntent.h src/application/head/intents/FireAtIntent.cpp
        include/application/head/intents/UseSkillIntent.h src/application/head/intents/UseSkillIntent.cpp
        include/application/head/intents/ResumeMatchIntent.h src/application/head/intents/ResumeMatchIntent.cpp
        include/application/head/intents/QuitIntent.h src/application/head/intents/QuitIntent.cpp
        include/application/head/intents/SaveMatchIntent.h src/application/head/intents/SaveMatchIntent.cpp
        include/application/head/intents/LoadMatchIntent.h src/application/head/intents/LoadMatchIntent.cpp
        # Reading input, and turning it into actions
        include/application/head/input/Keystroke.h src/application/head/input/Keystroke.cpp
        include/application/head/input/InputEvent.h
        include/application/head/input/EventHandler.h
        include/application/head/input/EventRouter.h src/application/head/input/EventRouter.cpp
        include/application/head/input/MoveCursorEventHandler.h
        src/application/head/input/MoveCursorEventHandler.cpp
        include/application/head/input/MenuEventHandlers.h src/application/head/input/MenuEventHandlers.cpp
        include/application/head/input/SaveMatchEventHandler.h
        src/application/head/input/SaveMatchEventHandler.cpp
        include/application/head/input/PlacementEventHandlers.h
        src/application/head/input/PlacementEventHandlers.cpp
        include/application/head/input/BattleEventHandlers.h src/application/head/input/BattleEventHandlers.cpp
        # How things are drawn
        include/application/head/views/Frame.h src/application/head/views/Frame.cpp
        include/application/head/views/GameView.h
        include/application/head/views/CellAppearance.h src/application/head/views/CellAppearance.cpp
        include/application/head/views/CoordinateLabel.h src/application/head/views/CoordinateLabel.cpp
        include/application/head/views/BoardView.h src/application/head/views/BoardView.cpp
        include/application/head/views/KeyHint.h src/application/head/views/KeyHint.cpp
        include/application/head/views/EventNarration.h src/application/head/views/EventNarration.cpp
        include/application/head/views/MenuView.h src/application/head/views/MenuView.cpp
        include/application/head/views/PlacementView.h src/application/head/views/PlacementView.cpp
        include/application/head/views/BattleView.h src/application/head/views/BattleView.cpp
        include/application/head/views/ViewFactory.h
        # ... the one place that knows both how FTXUI draws and how we do
        include/application/head/views/ftxui_bridge/FtxuiPalette.h
        src/application/head/views/ftxui_bridge/FtxuiPalette.cpp
        include/application/head/views/ftxui_bridge/FtxuiView.h
        src/application/head/views/ftxui_bridge/FtxuiView.cpp
        # ... and how they are drawn when there is no interactive terminal to draw into
        include/application/head/views/plain/PlainFrame.h src/application/head/views/plain/PlainFrame.cpp
        include/application/head/views/plain/PlainBoardGrid.h
        src/application/head/views/plain/PlainBoardGrid.cpp
        include/application/head/views/plain/PlainMenuView.h src/application/head/views/plain/PlainMenuView.cpp
        include/application/head/views/plain/PlainPlacementView.h
        src/application/head/views/plain/PlainPlacementView.cpp
        include/application/head/views/plain/PlainBattleView.h
        src/application/head/views/plain/PlainBattleView.cpp
        # A screen: its state, its view and what its keys do, wired together
        include/application/head/screens/Screen.h
        include/application/head/screens/ScreenKind.h
        include/application/head/screens/ScreenNavigator.h src/application/head/screens/ScreenNavigator.cpp
        include/application/head/screens/MenuState.h
        include/application/head/screens/MenuScreen.h src/application/head/screens/MenuScreen.cpp
        include/application/head/screens/PlacementState.h src/application/head/screens/PlacementState.cpp
        include/application/head/screens/PlacementScreen.h src/application/head/screens/PlacementScreen.cpp
        include/application/head/screens/BattleState.h src/application/head/screens/BattleState.cpp
        include/application/head/screens/BattleScreen.h src/application/head/screens/BattleScreen.cpp
        # Where it all runs
        include/application/head/host/Shell.h
        include/application/head/host/TuiShell.h src/application/head/host/TuiShell.cpp
        include/application/head/host/TerminalShell.h src/application/head/host/TerminalShell.cpp
)

# Tests setup
if (BUILD_TESTING)
    add_executable(warships_head_tests tests/main.cpp)
    target_link_libraries(warships_head_tests PRIVATE GTest::gtest_main warships_head)
    include(GoogleTest)
    gtest_discover_tests(warships_head_tests)
endif()

# Expose settings
target_include_directories(warships_head
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link dependencies
target_link_libraries(warships_head
        PUBLIC warships_core warships_flow warships_persistence ftxui::ftxui cxx_setup
)
