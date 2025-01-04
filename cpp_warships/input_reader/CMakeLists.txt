include_directories(./ ./src/ ./src/command ./src/config ./src/console)

add_library(input_reader
        src/command/CommandInputReader.cpp src/command/CommandInputReader.h
        src/config/ConfigInputReader.cpp src/config/ConfigInputReader.h
        src/console/ConsoleInputReader.cpp src/console/ConsoleInputReader.h)

if (BUILD_TESTING)
    add_executable(input_reader_tests tests/main.cpp)
    target_link_libraries(input_reader_tests PRIVATE GTest::gtest_main)
    include(GoogleTest)
    gtest_discover_tests(input_reader_tests)
endif()

target_link_libraries(input_reader PUBLIC cxx_setup)