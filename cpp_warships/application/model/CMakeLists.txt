# Compile files
add_library(warships_model
        # What can go wrong here
        include/application/model/errors/ModelExceptions.h src/application/model/errors/ModelExceptions.cpp
        # What the player asks for, and who is listening
        include/application/model/events/GameEvent.h
        include/application/model/events/EventQueue.h src/application/model/events/EventQueue.cpp
        include/application/model/events/EventScope.h
        include/application/model/events/EventHandler.h
        include/application/model/events/EventRouter.h src/application/model/events/EventRouter.cpp
        # What the game currently is
        include/application/model/BattleJournal.h src/application/model/BattleJournal.cpp
        include/application/model/MatchInPlay.h src/application/model/MatchInPlay.cpp
        # What can be done to it
        include/application/model/behaviors/MatchBehavior.h src/application/model/behaviors/MatchBehavior.cpp
        include/application/model/behaviors/SaveBehavior.h src/application/model/behaviors/SaveBehavior.cpp
        # What can be done about an ask, and in what order
        include/application/model/intents/IntentResult.h src/application/model/intents/IntentResult.cpp
        include/application/model/intents/GameIntent.h
        include/application/model/intents/MatchIntents.h src/application/model/intents/MatchIntents.cpp
        include/application/model/intents/SessionIntents.h src/application/model/intents/SessionIntents.cpp
        include/application/model/intents/IntentFactory.h src/application/model/intents/IntentFactory.cpp
        include/application/model/intents/IntentProcessor.h src/application/model/intents/IntentProcessor.cpp
        include/application/model/scenarios/IntendedGameScenario.h
        include/application/model/scenarios/SequenceScenario.h src/application/model/scenarios/SequenceScenario.cpp
        include/application/model/scenarios/ScenarioQueue.h
        include/application/model/scenarios/SyncScenarioQueue.h src/application/model/scenarios/SyncScenarioQueue.cpp
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
