include_directories(./ src/)

add_library(saves
        src/GameSaveCreator.cpp src/GameSaveCreator.h
        src/JsonFileHandler.cpp src/JsonFileHandler.h
        src/Serializer.cpp src/Serializer.h
        src/SerializerFactory.h
        src/SerializerGameField.cpp src/SerializerGameField.h
        src/SerializerManager.cpp src/SerializerManager.h
        src/SerializerMatchSettings.cpp src/SerializerMatchSettings.h
        src/SerializerSimplifyObjects.cpp src/SerializerSimplifyObjects.h
        src/SerializerSkillManager.cpp src/SerializerSkillManager.h)

if (BUILD_TESTING)
    add_executable(saves_tests tests/main.cpp)
    target_link_libraries(saves_tests PRIVATE GTest::gtest_main)
    include(GoogleTest)
    gtest_discover_tests(saves_tests)
endif()

target_link_libraries(saves PUBLIC cxx_setup)