# Compile files
add_library(warships_core
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
    add_executable(warships_core_tests tests/main.cpp)
    target_link_libraries(warships_core_tests PRIVATE GTest::gtest_main warships_core)
    include(GoogleTest)
    gtest_discover_tests(warships_core_tests)
endif()

# Expose settings
target_include_directories(warships_core
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link dependencies. warships_core is pure rules: it deliberately depends on no
# project library, so it stays free of I/O and of any UI toolkit.
target_link_libraries(warships_core PUBLIC cxx_setup)
