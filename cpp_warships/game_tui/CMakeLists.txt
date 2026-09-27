# Compile files
add_library(game_tui
        # Shared vocabulary
        include/game_tui/Color.h
        include/game_tui/Theme.h src/game_tui/Theme.cpp
        include/game_tui/Queries.h
        # What the session currently is
        include/game_tui/session/Application.h src/game_tui/session/Application.cpp
        include/game_tui/session/BattleJournal.h src/game_tui/session/BattleJournal.cpp
        # What a screen may ask for
        include/game_tui/intents/Intent.h
        include/game_tui/intents/IntentContext.h
        include/game_tui/intents/ShowScreenIntent.h src/game_tui/intents/ShowScreenIntent.cpp
        include/game_tui/intents/StartMatchIntent.h src/game_tui/intents/StartMatchIntent.cpp
        include/game_tui/intents/ChangeThemeIntent.h src/game_tui/intents/ChangeThemeIntent.cpp
        include/game_tui/intents/PlaceShipIntent.h src/game_tui/intents/PlaceShipIntent.cpp
        include/game_tui/intents/RemoveShipIntent.h src/game_tui/intents/RemoveShipIntent.cpp
        include/game_tui/intents/ShuffleFleetIntent.h src/game_tui/intents/ShuffleFleetIntent.cpp
        include/game_tui/intents/BeginBattleIntent.h src/game_tui/intents/BeginBattleIntent.cpp
        include/game_tui/intents/FireAtIntent.h src/game_tui/intents/FireAtIntent.cpp
        include/game_tui/intents/UseSkillIntent.h src/game_tui/intents/UseSkillIntent.cpp
        include/game_tui/intents/ResumeMatchIntent.h src/game_tui/intents/ResumeMatchIntent.cpp
        include/game_tui/intents/QuitIntent.h src/game_tui/intents/QuitIntent.cpp
        include/game_tui/intents/SaveMatchIntent.h src/game_tui/intents/SaveMatchIntent.cpp
        include/game_tui/intents/LoadMatchIntent.h src/game_tui/intents/LoadMatchIntent.cpp
        # Reading input, and turning it into actions
        include/game_tui/input/Keystroke.h src/game_tui/input/Keystroke.cpp
        include/game_tui/input/InputEvent.h
        include/game_tui/input/EventHandler.h
        include/game_tui/input/EventRouter.h src/game_tui/input/EventRouter.cpp
        include/game_tui/input/MoveCursorEventHandler.h
        src/game_tui/input/MoveCursorEventHandler.cpp
        include/game_tui/input/MenuEventHandlers.h src/game_tui/input/MenuEventHandlers.cpp
        include/game_tui/input/SaveMatchEventHandler.h
        src/game_tui/input/SaveMatchEventHandler.cpp
        include/game_tui/input/PlacementEventHandlers.h
        src/game_tui/input/PlacementEventHandlers.cpp
        include/game_tui/input/BattleEventHandlers.h src/game_tui/input/BattleEventHandlers.cpp
        # How things are drawn
        include/game_tui/views/Frame.h src/game_tui/views/Frame.cpp
        include/game_tui/views/GameView.h
        include/game_tui/views/CellAppearance.h src/game_tui/views/CellAppearance.cpp
        include/game_tui/views/CoordinateLabel.h src/game_tui/views/CoordinateLabel.cpp
        include/game_tui/views/BoardView.h src/game_tui/views/BoardView.cpp
        include/game_tui/views/KeyHint.h src/game_tui/views/KeyHint.cpp
        include/game_tui/views/EventNarration.h src/game_tui/views/EventNarration.cpp
        include/game_tui/views/MenuView.h src/game_tui/views/MenuView.cpp
        include/game_tui/views/PlacementView.h src/game_tui/views/PlacementView.cpp
        include/game_tui/views/BattleView.h src/game_tui/views/BattleView.cpp
        include/game_tui/views/ViewFactory.h
        # ... the one place that knows both how FTXUI draws and how we do
        include/game_tui/views/ftxui_bridge/FtxuiPalette.h
        src/game_tui/views/ftxui_bridge/FtxuiPalette.cpp
        include/game_tui/views/ftxui_bridge/FtxuiView.h
        src/game_tui/views/ftxui_bridge/FtxuiView.cpp
        # ... and how they are drawn when there is no interactive terminal to draw into
        include/game_tui/views/plain/PlainFrame.h src/game_tui/views/plain/PlainFrame.cpp
        include/game_tui/views/plain/PlainBoardGrid.h
        src/game_tui/views/plain/PlainBoardGrid.cpp
        include/game_tui/views/plain/PlainMenuView.h src/game_tui/views/plain/PlainMenuView.cpp
        include/game_tui/views/plain/PlainPlacementView.h
        src/game_tui/views/plain/PlainPlacementView.cpp
        include/game_tui/views/plain/PlainBattleView.h
        src/game_tui/views/plain/PlainBattleView.cpp
        # A screen: its state, its view and what its keys do, wired together
        include/game_tui/screens/Screen.h
        include/game_tui/screens/ScreenKind.h
        include/game_tui/screens/ScreenNavigator.h src/game_tui/screens/ScreenNavigator.cpp
        include/game_tui/screens/MenuState.h
        include/game_tui/screens/MenuScreen.h src/game_tui/screens/MenuScreen.cpp
        include/game_tui/screens/PlacementState.h src/game_tui/screens/PlacementState.cpp
        include/game_tui/screens/PlacementScreen.h src/game_tui/screens/PlacementScreen.cpp
        include/game_tui/screens/BattleState.h src/game_tui/screens/BattleState.cpp
        include/game_tui/screens/BattleScreen.h src/game_tui/screens/BattleScreen.cpp
        # Where it all runs
        include/game_tui/host/Shell.h
        include/game_tui/host/TuiShell.h src/game_tui/host/TuiShell.cpp
        include/game_tui/host/TerminalShell.h src/game_tui/host/TerminalShell.cpp
)

# Tests setup
if (BUILD_TESTING)
    add_executable(game_tui_tests tests/main.cpp)
    target_link_libraries(game_tui_tests PRIVATE GTest::gtest_main game_tui)
    include(GoogleTest)
    gtest_discover_tests(game_tui_tests)
endif()

# Expose settings
target_include_directories(game_tui
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link dependencies
target_link_libraries(game_tui
        PUBLIC game_core game_flow game_persistence ftxui::ftxui cxx_setup
)
