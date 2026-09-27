# Compile files
add_library(warships_model
        # What can go wrong here
        include/application/model/errors/ModelExceptions.h src/application/model/errors/ModelExceptions.cpp
        # What the game currently is
        include/application/model/BattleJournal.h src/application/model/BattleJournal.cpp
        include/application/model/MatchInPlay.h src/application/model/MatchInPlay.cpp
        # What can be done to it
        include/application/model/behaviors/MatchBehavior.h src/application/model/behaviors/MatchBehavior.cpp
        include/application/model/behaviors/SaveBehavior.h src/application/model/behaviors/SaveBehavior.cpp
        # The game, and the reach an intent is given into it
        include/application/model/WarshipsGame.h src/application/model/WarshipsGame.cpp
        include/application/model/ApplicationContext.h src/application/model/ApplicationContext.cpp
)

# Tests setup
if (BUILD_TESTING)
    add_executable(warships_model_tests tests/main.cpp)
    target_link_libraries(warships_model_tests PRIVATE GTest::gtest_main warships_model)
    include(GoogleTest)
    gtest_discover_tests(warships_model_tests)
endif()

# Expose settings
target_include_directories(warships_model
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link dependencies. The model decides what happens and stores it; it draws nothing
# and reads no input, so no drawing library appears here or below.
target_link_libraries(warships_model
        PUBLIC warships_core warships_flow warships_persistence cxx_setup
)
