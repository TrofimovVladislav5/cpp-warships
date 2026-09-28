# Compile files
add_library(warships_core
        # What can go wrong, and the root every other layer hangs its errors from
        include/application/core/errors/ErrorLayer.h src/application/core/errors/ErrorLayer.cpp
        include/application/core/errors/WarshipsException.h src/application/core/errors/WarshipsException.cpp
        # Value types
        include/application/core/Coordinate.h
        include/application/core/Direction.h
        include/application/core/Outcomes.h
        include/application/core/Segment.h src/application/core/Segment.cpp
        # Entities
        include/application/core/Ship.h src/application/core/Ship.cpp
        include/application/core/Board.h src/application/core/Board.cpp
        # Rules configuration
        include/application/core/FleetComposition.h src/application/core/FleetComposition.cpp
        include/application/core/MatchSettings.h src/application/core/MatchSettings.cpp
)

# Tests setup
if (BUILD_TESTING)
    add_executable(warships_core_tests
            tests/BoardTests.cpp
            tests/CoordinateTests.cpp
            tests/ErrorsTests.cpp
            tests/FleetCompositionTests.cpp
            tests/MatchSettingsTests.cpp
            tests/SegmentTests.cpp
            tests/ShipTests.cpp)
    target_link_libraries(warships_core_tests PRIVATE GTest::gtest_main warships_core)
    include(GoogleTest)
    gtest_discover_tests(warships_core_tests)
endif()

# Expose settings
target_include_directories(warships_core
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(warships_core PUBLIC cxx_setup)
