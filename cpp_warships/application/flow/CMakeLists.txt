# Compile files
add_library(warships_flow
        # Vocabulary
        include/application/flow/Participant.h
        include/application/flow/SkillKind.h
        include/application/flow/MatchEvent.h
        include/application/flow/MatchPhase.h
        include/application/flow/AttackOutcomeBehaviour.h src/application/flow/AttackOutcomeBehaviour.cpp
        include/application/flow/RandomEngine.h src/application/flow/RandomEngine.cpp
        # Match parts
        include/application/flow/EventLog.h src/application/flow/EventLog.cpp
        include/application/flow/TurnOrder.h src/application/flow/TurnOrder.cpp
        include/application/flow/ShotStrength.h src/application/flow/ShotStrength.cpp
        include/application/flow/SkillQueue.h src/application/flow/SkillQueue.cpp
        include/application/flow/SkillManager.h src/application/flow/SkillManager.cpp
        include/application/flow/SkillBehaviour.h src/application/flow/SkillBehaviour.cpp
        include/application/flow/PlacementPlan.h src/application/flow/PlacementPlan.cpp
        include/application/flow/AiOpponent.h src/application/flow/AiOpponent.cpp
        include/application/flow/Match.h src/application/flow/Match.cpp
)

# Tests setup
if (BUILD_TESTING)
    add_executable(warships_flow_tests tests/main.cpp)
    target_link_libraries(warships_flow_tests PRIVATE GTest::gtest_main warships_flow)
    include(GoogleTest)
    gtest_discover_tests(warships_flow_tests)
endif()

# Expose settings
target_include_directories(warships_flow
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link dependencies. warships_flow orchestrates the rules and, like warships_core,
# performs no input or output of its own.
target_link_libraries(warships_flow PUBLIC warships_core cxx_setup)
