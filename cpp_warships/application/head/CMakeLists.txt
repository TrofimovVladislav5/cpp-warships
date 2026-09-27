# Compile files
add_library(warships_head
        # What can go wrong here
        include/application/head/errors/PresentationExceptions.h src/application/head/errors/PresentationExceptions.cpp
        # Shared vocabulary
        include/application/head/Color.h
        include/application/head/Theme.h src/application/head/Theme.cpp
        include/application/head/Queries.h
        include/application/head/ThemeSelection.h src/application/head/ThemeSelection.cpp
        include/application/head/PresentationContext.h src/application/head/PresentationContext.cpp
        # Reading input: raw strokes in, asks in the game's own words out
        include/application/head/input/Keystroke.h src/application/head/input/Keystroke.cpp
        include/application/head/input/GridGeometry.h src/application/head/input/GridGeometry.cpp
        include/application/head/input/ScreenInput.h
        include/application/head/input/MenuInput.h src/application/head/input/MenuInput.cpp
        include/application/head/input/PlacementInput.h src/application/head/input/PlacementInput.cpp
        include/application/head/input/BattleInput.h src/application/head/input/BattleInput.cpp
        include/application/head/input/EventBus.h src/application/head/input/EventBus.cpp
        include/application/head/input/EventPipeline.h src/application/head/input/EventPipeline.cpp
        # ... and what the game does about them
        include/application/head/input/handlers/SessionHandlers.h
        src/application/head/input/handlers/SessionHandlers.cpp
        include/application/head/input/handlers/PlacementHandlers.h
        src/application/head/input/handlers/PlacementHandlers.cpp
        include/application/head/input/handlers/BattleHandlers.h
        src/application/head/input/handlers/BattleHandlers.cpp
        # How things are drawn
        include/application/head/views/Frame.h src/application/head/views/Frame.cpp
        include/application/head/views/Renderer.h
        include/application/head/views/RendererSet.h src/application/head/views/RendererSet.cpp
        include/application/head/views/Notices.h src/application/head/views/Notices.cpp
        include/application/head/views/CellAppearance.h src/application/head/views/CellAppearance.cpp
        include/application/head/views/CoordinateLabel.h src/application/head/views/CoordinateLabel.cpp
        include/application/head/views/BoardView.h src/application/head/views/BoardView.cpp
        include/application/head/views/KeyHint.h src/application/head/views/KeyHint.cpp
        include/application/head/views/EventNarration.h src/application/head/views/EventNarration.cpp
        include/application/head/views/MenuView.h src/application/head/views/MenuView.cpp
        include/application/head/views/PlacementView.h src/application/head/views/PlacementView.cpp
        include/application/head/views/BattleView.h src/application/head/views/BattleView.cpp
        # ... the one place that knows both how FTXUI draws and how we do
        include/application/head/views/ftxui_bridge/FtxuiPalette.h
        src/application/head/views/ftxui_bridge/FtxuiPalette.cpp
        include/application/head/views/ftxui_bridge/FtxuiView.h
        src/application/head/views/ftxui_bridge/FtxuiView.cpp
        include/application/head/views/ftxui_bridge/FtxuiNotices.h
        src/application/head/views/ftxui_bridge/FtxuiNotices.cpp
        # ... and how they are drawn when there is no interactive terminal to draw into
        include/application/head/views/plain/PlainFrame.h src/application/head/views/plain/PlainFrame.cpp
        include/application/head/views/plain/PlainBoardGrid.h
        src/application/head/views/plain/PlainBoardGrid.cpp
        include/application/head/views/plain/PlainMenuView.h src/application/head/views/plain/PlainMenuView.cpp
        include/application/head/views/plain/PlainPlacementView.h
        src/application/head/views/plain/PlainPlacementView.cpp
        include/application/head/views/plain/PlainBattleView.h
        src/application/head/views/plain/PlainBattleView.cpp
        # Which screen is showing, and what the interface remembers of its own
        include/application/head/screens/ScreenKind.h
        include/application/head/state/MenuState.h
        include/application/head/state/PlacementState.h src/application/head/state/PlacementState.cpp
        include/application/head/state/BattleState.h src/application/head/state/BattleState.cpp
        include/application/head/state/PresentationState.h
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
        PUBLIC warships_core warships_flow warships_persistence warships_model ftxui::ftxui cxx_setup
)
